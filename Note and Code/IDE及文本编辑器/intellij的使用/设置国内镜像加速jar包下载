    当我们使用maven从远程仓库下载jar包到本地时,由于默认的远程仓库是国外的仓库,下载的速度会非常慢
    为了解决这个问题,我们可以在 maven settings 文件进行修改,添加国内的仓库镜像,这样就换成了国内的仓库,
大大提升了jar包的下载速度.
配置方法:
找到 settings中的 <mirrors></mirrors>标签,在中间加入
<mirror>
    <id>nexus-aliyun</id>
    <mirrorOf>central</mirrorOf>
    <name>Nexus aliyun</name>
    <url>http://maven.aliyun.com/nexus/content/repositories/central</url>
</mirror>

1. 注意,这样做会有一些小问题,就是在使用 Intellij 的 maven jar包索引时版本提示并不完整,很多版本都不能
提示补全,不过很多版本虽然不能提示补全,但还是能从仓库下载

2.不要使用

<mirror>
    <id>nexus-aliyun</id>
    <mirrorOf>central</mirrorOf>
    <name>Nexus aliyun</name>
    <url>http://maven.aliyun.com/nexus/content/groups/public</url>
</mirror>
虽然也可以达到加速下载,但在Intellij中不能建立索引,使用起来很不方便
