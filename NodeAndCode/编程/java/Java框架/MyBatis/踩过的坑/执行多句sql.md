如果要在一个标签中引入多条sql语句,像下面那样
```xml
<delete id="deleteSeries" parameterType="int">
    DELETE FROM Image WHERE series_id = #{id};
    DELETE FROM Article WHERE series_id = #{id};
    DELETE FROM Series WHERE id = #{id};
</delete>
```
mybatis 是默认不支持的,应该在配置mysql 的 url 时显式设置 allowMultiQueries=true  
如
```bash
jdbc:mysql://xx.xx.xx:3306/xxxxx?characterEncoding=utf-8&autoReconnect=true&failOverReadOnly=false&allowMultiQueries=true
```
