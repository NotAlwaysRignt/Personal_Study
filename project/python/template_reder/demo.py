from Template import Template

text = r'''
{%for user in uers %}
    {{user.name|upper}}!
    {% if user.phone %}
        the phone number is: {{ user.phone }}
    {% endif %}
{%endfor%}
'''
class User:
    def __init__(self,name,phone = ""):
        self.name = name
        self.phone = phone

# Now you can render the HTML,besides string replacing, you can also add some rules by adding customized fuction
if __name__ == '__main__':
    t = Template()
    print(t.render_template(text,uers =[User("jack","123456"),User("bob"),User("Lily","789456")],upper = str.upper))
