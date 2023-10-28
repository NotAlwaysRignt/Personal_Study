使用 IDE 可以加快 HTML 的编写,一些IDE,比如 Emmet支持一些特殊的语法以支持高效地生成 HTML 标签,下面对这些语法做简要介绍:  
更多详细用法可以参考文章:http://www.iteye.com/news/27580  
1. "标签名称"+\#或"."+TAB键  
如:"div#test"+"TAB"键生成&lt;div id="test"&gt;&lt;/div&gt;   
"div.test"+"TAB"生成&lt;div class="test"&gt;&lt;/div&gt; 
2. 标签名称>标签名称+Tab键  
ul&gt;li 生成&lt;ul&gt;&lt;li&gt;&lt;/li&gt;&lt;/ul&gt;  
3. "标签\*数字"  +Tab键    
li \* 4 + Tab 生成4个&lt;li&gt;标签  
4. 定义HTML元素的内容和属性。你可以通过输入h1{foo} + "Tab"键 和 a\[href="\#" myattr="unkown"\]\{hello\} + "Tab"，就可以分别自动生成如下代码:  
```HTML
<h1>foo</h1>  
 <a href="#" myattr="unkown">hello</a>
```
5. 你可以通过嵌套和括号来快速生成一些代码块，比如输入(.foo>h1)+(.bar>h2)，会自动生成如下代码：  
```HTML
    <div class="foo">  
      <h1></h1>  
    </div>  
    <div class="bar">  
      <h2></h2>  
    </div>  
```
### 随机生成文本
在IDEA或一些前端编辑器中,输入"lorem"按下"Tab"键,就会随机生成一段文本,文本的内容无任何意义, "lorem"后面加数字,再按TAB键可以随机生成指定个数的单词,比如"lorem4"+"TAB"会生成4个单词 
