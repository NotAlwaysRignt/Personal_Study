不同浏览器的默认样式不同,为了不让浏览器的默认行为影响我们的样式,将所有元素的属性先统一,比如 h1 标签的字体将不再加粗,大小将和p标签一致,我们需要重新定义h1标签的各种行为
### CSS重置(CSS reset)
所有元素不使用浏览器的默认样式,像 h1 这样的标签浏览器也不会去渲染,必须要自己去定义其样式  
元素的位置(内边距,外边距也将归0),元素的位置也需要自己定义  
**缺点**: 全部革除,全部都要自己定义,会增大自己的工作量,而且可能会遗漏某些元素的定义  
### Normalize.css
Normalize.css 只是一个很小的CSS文件,如果说CSS重置是革命派,主张推倒一切重来,那Normalize.css就是改良派,它保持了许多默认的浏览器样式,并修复了常见的桌面端和移动端浏览器的bug(比如HTML5元素的显示设置、预格式化文字的font-size问题)  
使用方式,直接到官网下载css文件(打开css内容的网页,在浏览器上点击右键,另存为)并引入即可  
