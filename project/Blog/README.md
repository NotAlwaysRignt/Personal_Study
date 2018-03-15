## 此版本功能仍然不完善,不建议下载使用

## 如何运行程序
要让程序在自己的服务器上跑起来,首先找到 sql/BlogScheme.sql,建立好数据库创建好表,然后必须在三个文件夹里修改自己的配置,配置完毕后,在指定的目录结构下存放 markdown文件(普通文件也可以,如果是markdown文件前端会做渲染) 

####  修改博客文件路径  
找到**main/java/cn.chao.SpringConfig.BeanConfig.java**,打开后找到 blog_file_url 以及 blog_image_url 变量  ,blog_file_url 存放博客文章根路径  
比如 blog_file_url="/home/static/posts",那么你的博客文章就都要存放到 posts 目录下,并且要遵循 Category(类别)/Series(该系列文章的名字)/Article(具体的文章,markdown文件)+img(这个文件夹存放文章引用的图片,在md文件中统一使用\!\[img/图片路径\])的形式指向这篇文章的图片  
blog_image_url 将用于指向一个临时用于存放图片文件的目录,在 blog_file_url中的所有图片都会被拷贝一份到这个目录中,并且命名必须与markdown的命名格式同一才能访问,比如markdown中同一使用\!\[img/图片路径\]),那么这个文件夹名称应该叫 img,这个目录可以是任意的,但一定要在  application.properties 文件里配置能够指向它以便网页能访问

#### 修改 application.properties  
其中 web.image_path=/home/thinker/code/java/Blog/static 要改为自己指定的目录 上面提到的  blog_image_url 目录的上一层目录,比如
blog_image_url = "/home/static/img",那么web.image_path="/home/static/i",如此一来网页在访问"img/图片路径"时才会找到图片(因为博客中统一使用\!\[img/图片路径\])格式,因此会被渲染成着一个格式)  

#### mybatis-config.xml 文件(配置数据库)
这个文件主要是配置自己数据库的信息,确保能连接上自己的数据库,要修改&lt;environments&gt;标签下的变量,包括引擎,数据库名,用户密码等  

### 目录结构 
假设 blog_file_url 指向 /posts 文件夹
那么文件目录应该遵循以下格式
/posts/category/series/img + article.md
category series 都是目录,必须创建,程序会逐层查找到series目录下存放自己的 markdown文件,并读入数据库中
category表示分类,"历史", "编程"  
series表示文章属于哪一系列,比如"java教程"  
article是 markdown 文件,可以有多个,img 是一个目录,里面用于存放所有markdown文件指向的图片,注意img目录不能有再下一级目录,否则这些目录的图片将不会被读取到  
