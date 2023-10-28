Strapdown官网: http://strapdownjs.com/  
Stapdownjs 是用javascript 编写的markdown解析器,它能够渲染 markdown 内容,并在浏览器上显示.还可以使用不同的bootstrap主题:  
strapdownjs 的使用方法很简单,只要在html文件里引入  
```HTML
<script src="http://strapdownjs.com/v/0.2/strapdown.js"></script> 
```  
之后在 xmp 标签内的markdown内容将会被渲染
```HTML
<xmp class="html" theme="united" style="display:none;">
  ...markdown文档代码
</xmp>  
```  
theme 属性指的是主题,不同主题最终渲染效果不同.上例中"united"就是其中一种主题.  具体有哪些主体请到官网查看  

官网提供了一个 HTML 示例,我们把 HTML 内容保存到本地的一个HTML文件中,然后用浏览器打开它,就可以观察效果了.    

因为上面的方法使用的是引入网上的 js 文件,因为是 js文件在国外网站.可能会出现加载比较慢的情况.所以我们可以把文件下载到本地.  

**注意**:引入strapdownjs 时**不能只引入 strapdown.js 文件,还需要引入特定的CSS**,因为它还会尝试在它设置的默认路径下寻找相应的CSS,如果找不到,则无法成功渲染.  
在官网之所以能渲染成功,是因为官网服务器保存了相应的CSS  
因此我们要去下载相应的CSS,**引入CSS时不需要把CSS文件保存在特定路径下,只要能够引入 strapdown.js就能工作**)  
我们还需要引入的CSS有  
* slate.min.css(这个与选择的主体有关,比如主题* 是united,则需要引入united.min.css)  
* strapdown.css  
* bootstrap-responsive.min.css(strapdown使用的是bootstrap2版本,我们可以添加bootstrap4,不会冲突)  
**要注意的是,用link引入上面三个CSS文件时顺序也要和上面保持一致!否则在用小屏幕时会显示出错**

　　那么我们怎么知道要引入这些CSS以及去哪里找到这些 CSS 呢?  
　　创建一个 HTML 文件,我们可以先引用网络上的 strapdown.js ,来完成一个简单的markdown渲染示例.  
　　在浏览器上打开这个HTML文件,如果我们的markdown内容能够被正常渲染,说明**strapdown.js 成功引入并正常工作.  
　　接下来我们使用浏览器的开发者工具,观察HTML文档里的 link标签(还要注意它们引入的顺序),就可以知道有哪些CSS文件被引入了,并可以通过其src属性找到CSS文件的网址,直接在浏览器中输入网址找到这些css文件,另存为我们的本地文件**  
我们可以选择官网的示例保存到本地的HTML文件中,打开看能否正常工作
```HTML
<!DOCTYPE html>
<html>
<title>Hello Strapdown</title>

<xmp theme="united" style="display:none;">
# Markdown text goes in here

## Chapter 1

Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore
et dolore magna aliqua. 

## Chapter 2

Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut
aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse
cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in
culpa qui officia deserunt mollit anim id est laborum.
</xmp>

<script src="http://strapdownjs.com/v/0.2/strapdown.js"></script>
</html>
```  