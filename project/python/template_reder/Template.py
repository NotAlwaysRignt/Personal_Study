import re

class TagLayer:
    def __init__(self, tag, between_for_tags, if_state):
        self.current_tag = tag
        self.if_tag = if_state
        self.between_for_tags = between_for_tags
        self.string = ''

    def get_current_tag(self):
        return self.current_tag
        
    def get_string(self):
        return self.string

    def set_string(self,s):
        self.string = s

    def if_state(self):
        return self.if_tag

    def is_between_for_tags(self):
        return self.between_for_tags

    def append_string(self,s):
        self.string = self.string + s

# a logic Stack
class Stack:
    element_num = 0
    stack = []
    def __init__(self):
        pass

    def is_empty(self):
        return self.element_num == 0

    def pop(self):
        if self.is_empty():
            raise IndexError("too much {%end%]")
        self.element_num = self.element_num - 1
        return self.stack.pop()

    def push(self,value):
        self.element_num = self.element_num + 1
        self.stack.append(value)

    def top(self):
        if self.is_empty():
            raise IndexError("too much {% end %}")
        return self.stack[-1]

class Template:

    def __init__(self):
        self.pattern = re.compile("({%.*?%}|{{.*?}}|{#.*?#})",re.S)
        self.stack = Stack()
        

    def render_template(self,html,**context):
        t = TagLayer('global', False, True)
        self.stack.push(t)
        tokens = self._parse_tags(html)
        self._parse_tokens(tokens,context)
        final_html = self.stack.pop().get_string()
        if not self.stack.is_empty():
            raise ValueError("the end tag and {% tag is not matched")
        return final_html

    def _parse_tags(self,html):
        return self.pattern.split(html)

    def _parse_tokens(self,tokens,context):
        for token in tokens:       
            current_layer = self.stack.top()
            between_for_tags = current_layer.is_between_for_tags()
            if_state = current_layer.if_state()
            
            if token.startswith("{#"):
                continue           
            elif token.startswith("{%"):
                self._logic_tag_parse(token, between_for_tags, if_state, context)
            elif token.startswith("{{"):
                self._variable_tag_parse(token, between_for_tags, if_state, context)
            else:
                self._html_parse(token, between_for_tags, if_state)

    # deal with "{% if xx %}  {% for a in b %} {% endif %} {% endfor %}"
    def _logic_tag_parse(self, token,  between_for_tags, if_state, context):
        
        control_sentence = token[2:-2].strip().split()
        sentence_len = len(control_sentence)        
                
        if control_sentence[0] == "if" and sentence_len == 2 :  #if
            self._if_tag_parse(token,between_for_tags,if_state,context)
                
        elif control_sentence[0] == "for" and sentence_len == 4: # for
            self._for_tag_parse(token,between_for_tags,if_state,context)
            
        elif control_sentence[0].startswith("end") and sentence_len == 1:  
            self._end_tag_parse(token,between_for_tags,if_state,context)
        else: # error
            raise ValueError("the sentence '%s' is wrong" %''.join(control_sentence))
            
    def _html_parse(self,token):
        current_layer = self.stack.top()
        if current_layer.is_between_for_tags():
            current_layer.append_string(token)
        elif current_layer.if_state() == True:
            current_layer.append_string(token)
        else: # not between_for_tags and if_state is False
            pass

    def _if_tag_parse(self,token,between_for_tags,if_state,context):
        if between_for_tags: 
            t = TagLayer("if",True,True)
            t.append_string(token)
            self.stack.push(t) 
        elif if_state: # between{%if A %} {% endif %} and A is True
            control_sentence = token[2:-2].strip().split()
            #result = context.get(control_sentence[1], 
            #                     ValueError("don't understand %s" % control_sentence[1]))
            result = self._variable_parse(control_sentence[1],context)
            if (type(result) == ValueError):
                raise result
                
            if result:
                t = TagLayer("if",False,True)
                self.stack.push(t)
            else:
                t = TagLayer("if",False,False)
                self.stack.push(t)
        
    def _for_tag_parse(self,token,between_for_tags,if_state,context):
        if between_for_tags: 
            t = TagLayer("for",True,if_state)
            t.append_string(token)
            self.stack.push(t) 
        else:
            if if_state:
                t = TagLayer("for", True, True)
                t.append_string(token)
                self.stack.push(t)
            else :
                t = TagLayer("for", True, False)
                self.stack.push(t)
        
    def _end_tag_parse(self,token,between_for_tags,if_state,context):
        control_sentence = token[2:-2].strip().split()
        t = self.stack.pop()

        if control_sentence[0][3:] == "if": # {% endif %}
            if not t.get_current_tag() == "if":
                raise ValueError("%s is mismatched" % token) 
            if between_for_tags:
                t.append_string("{%endif%}")
                self.stack.top().append_string(t.get_string())
            else:
                if if_state:
                    self.stack.top().append_string(t.get_string())            
                        
        elif control_sentence[0][3:] == "for":
            if not t.get_current_tag() == "for":
                raise ValueError("%s is mismatched" % token)
            
            if between_for_tags: 
                if self.stack.top().is_between_for_tags():
                    t.append_string("{%endfor%}")
                    self.stack.top().append_string(t.get_string())
                else:
                    if if_state: # 必有 t.if_state() == self.stack.top().if_state()
                        self._append_string_in_for_loop(t.get_string(),context)
                    
        else:
            raise ValueError("Don't unsterstand \'%s\'" % token)             
    
    def _variable_tag_parse(self, token, between_for_tags, if_state, context):
        stack_top = self.stack.top()
        if between_for_tags:
            stack_top.append_string(token)
        else:
            if if_state:
                variable_sentence = token[2:-2].strip()
                # "{{Bob.name|upper}}"提取出"Bob.name|upper"
                variable = self._variable_parse(variable_sentence,context)
                stack_top.append_string(variable)
        
    def _html_parse(self, token, between_for_tags, if_state):
        stack_top = self.stack.top()
        if between_for_tags:
            stack_top.append_string(token)
        else:
            if if_state:
                stack_top.append_string(token)                           
    
    def _append_string_in_for_loop(self, string_between_for_tag, context):
        tokens = self._parse_tags(string_between_for_tag) 
        # "{%for a in b%}"中提取"for a in b",re.split切割后存放在tokens[1],tokens[0]=""     
        for_sentence = tokens[1].strip()[2:-2] 
        words = for_sentence.split()
        iter_variable = context.get(words[3],
                    ValueError("undefined variable %s" % words[3]))

        if type(iter_variable) == ValueError:
            raise iter_variable
        for element in iter_variable:
            context[words[1]] = element
            self._parse_tokens(tokens[2:],context)
            del context[words[1]]
      

    def _variable_parse(self,variable_sentence,context):                  
        words = variable_sentence.split("|")
        filter_functions = words[1:]
        variable_name = words[0].split(".")  # "Bob.name"拆分为 ["Bob","name"]
        variable = context.get(variable_name[0],
                   ValueError("undefined variable %s" % variable_sentence))
        if type(variable) == ValueError:
            raise variable
        
        elif not variable:
            variable = '' # 如果variable存在但为 None或者空列表之类，则variable为空串
                
        else: # 如果variable_name只有一个元素，那么variable_name[1:] 为空[]
            for attr in variable_name[1:]:
                variable = getattr(variable,attr,
                      ValueError("cant't not get %s in \'%s\'"% (attr, variable_sentence)))
            
            for func in filter_functions:
                filter_func = context.get(func,
                      ValueError("cant't not get %s in \'%s\'"% (func, variable_sentence)))
                if not filter_func or type(filter_func) == ValueError:
                    raise filter_func
                else:
                    variable = filter_func(str(variable))      
        return str(variable)    


#以下代码作为测试用例
text = r"""
{%for user in uers %}
    {{user.name|upper|add_prefix}}!
    {% if user.phone %}
        the phone number is: {{ user.phone }}
    {% endif %}
{%endfor%}
"""
html = """
<p> for control sentence test</p>
{% for item in items %}
the element in this group:
    {%for element in  item %}
        {{element}}
    {% endfor %}
{% endfor %}
<p> the result is above</p>
"""

def add_prefix(string): # 使字符串的第一个字母大写
    string = 'The username is ' + string 
    return string

class User:
    def __init__(self,name,phone = ""):
        self.name = name
        self.phone = phone

if __name__ == '__main__':
    t = Template()
    print(t.render_template(text,uers =[User("jack","123456"),User("bob"),User("Lily","789456")],upper = str.upper,add_prefix = add_prefix))
    #print(t.render_template(html,items = [['a','b','c'], [1,2,3], [',','.','?']]))
