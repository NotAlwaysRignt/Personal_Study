# 这是一个仿制jinjia2模板的简化版 HTML 模板渲染器
1.支持变量的渲染 {{ value }}
2.支持逻辑控制语句 if for 
{% if condition %} {% endif %}
{% for item in items %} {% endfor %}
3.支持过滤器 {{ value|safe }}
需要自定义过滤器函数并传入

使用示例1,过滤器,自定义对象,条件语句,循环控制语句:
```python
text = r"""
{%for user in uers %}
    {{user.name|upper}}!
    {% if user.phone %}
        the phone number is: {{ user.phone }}
    {% endif %}
{%endfor%}
"""

class User:
    def __init__(self,name,phone = ""):
        self.name = name
        self.phone = phone

if __name__ == '__main__':
    t = Template()
    print(t.render_template(text,uers =[User("jack","123456"),User("bob"),User("Lily","789456")],upper = str.upper))
```

使用示例2,循环语句的嵌套  
```python
html = """
<p> for control sentence test</p>
{% for item in items %}
the element in this group:
    {%for element in  item %}
        {{element}}
    {% endfor %}
{% endfor %}
<p> the result is above</p>

if __name__ == '__main__':
    t = Template()
    print(t.render_template(html,items = [['a','b','c'], [1,2,3], [',','.','?']]))
"""
```
shiyanlou_sample.py为实验楼提供的代码,以此为参考自己重新进行了实现,避开了使用 exec 的使用
