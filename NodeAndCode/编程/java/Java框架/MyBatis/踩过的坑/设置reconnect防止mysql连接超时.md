mysql 默认一段时间没有任何操作后,连接会断开,这会导致如果长时间没有访问数据库,那么之后用户第一次访问时执行 mysql 语句会失败,而执行失败后mysql才开始重新连接,再下一次又会成功,以致出现非常奇怪的bug,解决办法:
在url 中,设置 autoReconnect=true  
如下:
```
jdbc.url =jdbc:mysql://localhost:3306/Blog?useSSL=false&characterEncoding=utf-8&autoReconnect=true&
```  
同时,在 XML 配置中添加 poolPingQuery 和 poolPingEnabled 选项
```XML
    <environments default="development">
        <environment id="development">
            <transactionManager type="JDBC"/>
            <dataSource type="POOLED">
                <property name="driver" value="com.mysql.jdbc.Driver"/>
                <!--<property name="url" value="jdbc:mysql://localhost:3306/Blog?useSSL=false&amp;characterEncoding=utf-8&amp;autoReconnect=true&amp;allowMultiQueries=true"/>-->
                <property name="url" value="${jdbc.url}"></property>
                <property name="username" value="${jdbc.username}"/>
                <property name="password" value="${jdbc.password}"/>
                <property name="poolPingQuery" value="SELECT NOW()" />
                <property name="poolPingEnabled" value="true" />
                <property name="poolPingConnectionsNotUsedFor" value="3600000"/>
            </dataSource>
        </environment>
    </environments>
```  
poolPingQuery	发送到数据的侦测查询，用来验证连接是否正常工作，并且准备接受请求。默认为“NO PING QUERY SET”，这会引起许多数据库驱动连接由一个错误信息而导致失败
poolPingEnabled	这是开启或禁用侦测查询，如果开启，必须用一个合法的SQL语句，设置poolPingQuery属性，默认值为false
poolPingConnectionsNotUsedFor	用来配置poolPingQuery多长时间被调用一次。可以被设置匹配标准的数据库链接超时时间，来避免不必要的侦测。默认值0（也就是所有链接每一时刻都被侦测到，但仅仅当poolPingEnabled为true时适用）。上面我设置为1小时,如果不设,则每执行一句sql都要执行 poolPingQuery,显然会降低效率
