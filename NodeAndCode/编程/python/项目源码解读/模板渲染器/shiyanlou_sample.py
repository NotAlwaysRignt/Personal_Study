"""A simple Python template renderer, for a nano-subset of Django syntax."""

# Coincidentally named the same as http://code.activestate.com/recipes/496702/

import re


class TempliteSyntaxError(ValueError):
    """Raised when a template has a syntax error."""
    pass


class CodeBuilder(object):
    """Build source code conveniently."""

    def __init__(self, indent=0):
        self.code = [] # code内存储的是字符串和CodeBuilder实例两种类型,如['print(',section,')']
        self.indent_level = indent
    
    # 这个__str__，使得在调用 str(self)时拿到了一个由self.code中所有字符拼接起来的字符串
    def __str__(self):
        return "".join(str(c) for c in self.code)
        #str(c)的原因是 c可能是一个字符串，也可能是一个 CodeBuilder 的实例

    def add_line(self, line):
        """Add a line of source to the code.
        Indentation and newline will be added for you, don't provide them.
        """
        self.code.extend([" " * self.indent_level, line, "\n"])

    def add_section(self):
        """Add a section, a sub-CodeBuilder."""
        section = CodeBuilder(self.indent_level)
        self.code.append(section)
        return section

    INDENT_STEP = 4      # PEP8 says so!

    def indent(self):
        """Increase the current indent for following lines."""
        self.indent_level += self.INDENT_STEP

    def dedent(self):
        """Decrease the current indent for following lines."""
        self.indent_level -= self.INDENT_STEP

    def get_globals(self):
        """Execute the code, and return a dict of globals it defines."""
        # A check that the caller really finished all the blocks they started.
        assert self.indent_level == 0
        # Get the Python source as a single string.
        python_source = str(self) # 此处会执行上面自定义的 __str__
        # Execute the source, defining globals, and return them.
        global_namespace = {}
        exec(python_source, global_namespace)
        return global_namespace


class Templite(object):
    def __init__(self, text, *contexts):
        '''
            text即渲染的html字符串，*context是一个tuple，里面的每一个元素都是且必须是字典类型
             
            渲染模板的核心思想是利用python 的exec()函数，这个函数可以将字符串转换为可以调用的
        python代码(前提是这个字符串遵从python的语法规则)
            
            __init__创建了一个CodeBuilder实例 code ，这个实例内部有一个列表变量code
            code.code 中会存储要执行的python代码(还未经过exec()转换)，html文本在被解析后也会被
        插入self.code中，作为python代码的一部分，最后再调用最终生成的python代码，生成最终渲染后的
        模板.
            code.code存储的内容如下：
            ['def render_function(context, do_dots):',
                section,#这是一个CodeBuilder类的实例,最终可以在''.join(self.code)时转换为字符串
             '  result = []',
             '  append_result = result.append',
             '  extend_result = result.extend',
             '  to_str = str',
             '  if c_%s:...'
             '      extend_result([%s])', # 或者为'append_result(%s)',%s为buffered里的所有内容
             '  for c_%s in %s:...'#....从 {%if ...%} 或 {% for ... in ...%}提取出来的语句
             '      extend_result([%s])',
             '  extend_result([%s])', #如果遇到普通变量就直接extend_result
             'return ''.join(result)']
             
             section 的__str__方法中有 return "".join(str(c) for c in self.code)
             section实例的 self.code中存储的数据为
             ['c_%s = context[%r]', #flush_output()函数调用时，会把所有从{{ }}中提取的变量
             'c_%s = context[%r]',  #变成python代码中名为 c_%s 的变量
             'c_%s = ...']
             # context[%r]最终是我们传入的字典，比如使用render(html,[{'a':1},{'b':2}])
                        #最终有context['a'] = 1,context['b'] = 2
             
             所以render_function真的很简单，创建一个列表[],把所有要替代的变量名对应的键值
             放在字典context里，
               
        '''
        self.context = {}
        for context in contexts:
            self.context.update(context)

        self.all_vars = set()
        self.loop_vars = set()

        # We construct a function in source form, then compile it and hold onto
        # it, and execute it to render the template.
        code = CodeBuilder()
        
        #定义一个函数render_function，整个函数的内容都作为字符串最终传入到
        code.add_line("def render_function(context, do_dots):")
        code.indent()
        vars_code = code.add_section()
        code.add_line("result = []")
        code.add_line("append_result = result.append")
        code.add_line("extend_result = result.extend")
        code.add_line("to_str = str")

        buffered = []
        
        # 把buffered里存储的所有变量转换为字符串"extend_result([%s])" 输出到 code.code中
        def flush_output(): 
            """Force `buffered` to the code builder."""
            if len(buffered) == 1:
                code.add_line("append_result(%s)" % buffered[0])
            elif len(buffered) > 1:
                code.add_line("extend_result([%s])" % ", ".join(buffered))
            del buffered[:] 
            # 清空buffered，注意后面的'[:]'，如果没有这个buffered将变为None而不是空的[]

        ops_stack = []

        # Split the text to form a list of tokens.
        tokens = re.split(r"(?s)({{.*?}}|{%.*?%}|{#.*?#})", text)

        for token in tokens:
            if token.startswith('{#'):
                # Comment: ignore it and move on.
                continue
            elif token.startswith('{{'):
                # An expression to evaluate.
                expr = self._expr_code(token[2:-2].strip())
                buffered.append("to_str(%s)" % expr)                                
                '''
                当读取条件控制语句{%时，先用flush_output把 buffered里的内容输出到code.code中
                即baffered里的所有变量如 html字符串 或者读到的{{变量}}，都以extend([%s])的形式
                输出到code中,flush_output 以后，接下来就往code 中添加条件控制语句，如 if xx:
                for ...:,则再添加一个缩进，接下来读到的除'{%'以外的任何内容，比如普通的html文本
                或{{变量}},都会加入buffered中，如果遇到了下一个嵌套的条件语句{%if %}或{% for%}，
                则继续将buffered输出，并增加缩进，如果遇到了{%end...%}，则先将buffered的内容输
                出，并减少一个缩进，这样就顺利实现了条件控制语句
                '''                
            elif token.startswith('{%'):
                # Action tag: split into words and parse further.
                flush_output() # 遇到 if for时先把前面的输出
                words = token[2:-2].strip().split() 
                # words可能的形式:["if","condition"] ["for", "var", "in", "list"]
                if words[0] == 'if':
                    # An if statement: evaluate the expression to determine if.
                    if len(words) != 2:
                        self._syntax_error("Don't understand if", token)
                    ops_stack.append('if')
                    code.add_line("if %s:" % self._expr_code(words[1]))
                    code.indent()
                elif words[0] == 'for':
                    # A loop: iterate over expression result.
                    if len(words) != 4 or words[2] != 'in':
                        self._syntax_error("Don't understand for", token)
                    ops_stack.append('for')
                    self._variable(words[1], self.loop_vars)
                    code.add_line(
                        "for c_%s in %s:" % (
                            words[1],
                            self._expr_code(words[3])
                        )
                    )
                    code.indent()
                elif words[0].startswith('end'):
                    # Endsomething.  Pop the ops stack.
                    if len(words) != 1:
                        self._syntax_error("Don't understand end", token)
                    end_what = words[0][3:]
                    if not ops_stack:
                        self._syntax_error("Too many ends", token)
                    start_what = ops_stack.pop()
                    if start_what != end_what:
                        self._syntax_error("Mismatched end tag", end_what)
                    code.dedent()
                else:
                    self._syntax_error("Don't understand tag", words[0])
            else:
                #如果读到的是普通的 html文本字符串，直接存到buffered中
                if token:
                    buffered.append(repr(token)) 
                    #repr("hello"),输入"'hello'"，在字符串中添加引号
                    
        #ops_stack作为栈，用来检验end 和条件控制语句 if for是否完全匹配，经过上面的步骤
        #如果 end和if,for相匹配，那么ops_stack应该为空，若不为空，那么肯定是if,for 和end数量不匹配
        if ops_stack:
            self._syntax_error("Unmatched action tag", ops_stack[-1])
        flush_output() 
        #将buffered中剩余的字符串都输出到code对象中
        #注意是剩余的，if for这些在flash_output中已经输出

        for var_name in self.all_vars - self.loop_vars:
            vars_code.add_line("c_%s = context[%r]" % (var_name, var_name))
            #"%r. %s" %'hello' %'hello' == "'hello', hello",%r调用__repr__方法
        code.add_line("return ''.join(result)")
        code.dedent()
        #至此code中的render_function便完成了
        self._render_function = code.get_globals()['render_function']
    
    #设 expr(意味expression) 为 "a.value|safe|upper"
    #则会传回字符串 "c_upper(c_safe(do_dots(c_a, 'name')))"
    #注意该函数无法对a["key"]形式的值解析,对于a={"key":"val"}，可用a.key，_variable方法会转换    
    def _expr_code(self, expr):
        """Generate a Python expression for `expr`."""
        if "|" in expr:
            pipes = expr.split("|") #则 pipes = ["a.value","safe","upper"]，过滤器和变量分离
            code = self._expr_code(pipes[0]) # 这里迭代，将 "a.name"进行继续解析
            for func in pipes[1:]: #pipes[1:]内存储的都是过滤器
                self._variable(func, self.all_vars) #将所有的过滤器添加到 all_vars中
                code = "c_%s(%s)" % (func, code) #形成c_func1(c_func2(变量名))的形式
        elif "." in expr:
            dots = expr.split(".")
            code = self._expr_code(dots[0])
            args = ", ".join(repr(d) for d in dots[1:])
            #若dots为["a","name"]，则args = "'name'"
            #若dots为["a","name","value"]则 args = "'name', 'value'"
            code = "do_dots(%s, %s)" % (code, args) #code ="do_dots(a,'name')"
        else: #当变量已经无法再进一步拆分,比如只拆分到一个"a"时，将变为"c_a"返回
            self._variable(expr, self.all_vars)
            code = "c_%s" % expr
        return code

    def _syntax_error(self, msg, thing):
        """Raise a syntax error using `msg`, and showing `thing`."""
        raise TempliteSyntaxError("%s: %r" % (msg, thing))

    #检查 变量name命名是否合法，如果合法，就添加到 vars_set中，vars_set传入的应是一个set变量
    def _variable(self, name, vars_set):
        """Track that `name` is used as a variable.
        Adds the name to `vars_set`, a set of variable names.
        Raises an syntax error if `name` is not a valid name.
        """
        if not re.match(r"[_a-zA-Z][_a-zA-Z0-9]*$", name):
            self._syntax_error("Not a valid name", name)
        vars_set.add(name)

    #我们传给Templatile的构造函数参数 contexts(字典)，以及self._do_dots这个函数并没有转换为
    #code.code的字符串作为python代码的一部分，而是直接作为code.code最终生成函数的参数
    def render(self, context=None):
        """Render this template by applying it to `context`.
        `context` is a dictionary of values to use in this rendering.
        """
        # Make the complete context we'll use.
        render_context = dict(self.context)
        if context:
            render_context.update(context)
        return self._render_function(render_context, self._do_dots)
    
    def _do_dots(self, value, *dots):
        """Evaluate dotted expressions at runtime."""
        for dot in dots:
            try:
                value = getattr(value, dot)
            except AttributeError:
                value = value[dot] #如果value是个字段，则以value[dot]形式获取它
            if callable(value): # 如果 value.dot是可以调用的方法则调用它
                value = value()
        return value
        
        
if __name__=="__main__":
    from collections import namedtuple
    
    template_text = """
    <p>Welcome, {{user_name}}!</p>
    <p>Products:</p>
    <ul>
    {% for product in product_list %}
        <li>{{ product.name }}:
            {{ product.price|format_price|upper}}</li>
    {% endfor %}
    </ul>
    """

    Product = namedtuple("Product",["name", "price"])
    product_list = [Product("Apple", 1), Product("Fig", 1.5), Product("Pomegranate", 3.25)]

    def format_price(price):
        return "$%.2f" % price

    t = Templite(template_text, {"user_name":"Charlie", "product_list":product_list}, {"format_price":format_price},{'upper':str.upper})
    print(t.render())
