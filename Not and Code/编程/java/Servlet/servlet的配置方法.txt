一般一个url 对应一个<sevlet> + <servlet-mapping>
即一个url 对应一个servlet(自定义名字) 然后指名 相应url映射的 servlet名

<servlet>
<servlet-name>Hello</servlet-name>
<servlet-class> com.example.Helloservelt</servlet-class>
</servlet>

<servlet-mapping>
<servlet-name>Hello</servlet-name>
<url-pattern> /hello </url-pattern>
</servlet-mapping>