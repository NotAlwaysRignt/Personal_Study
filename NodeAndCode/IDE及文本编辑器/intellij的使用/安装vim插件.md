IDEA(Pycharm Clion等) 支持安装 vim 插件-- ideavim,在插件仓库(file>settings>Plugin)搜索vim 即可找到  

我们可以阅读插件上的说明了解这款插件的配置,一般来说,完成安装并启动就可以使用vim的基本功能了,在这款插件中,vim的分窗功能就可以实现 idea 的分窗,非常方便  

## vim 配置文件
在 ideavim 的0.46版本以后就可以在配置文件中配置自己的vim了  
具体做法是在~/ 下创建一个名为 .ideavimrc 的文件,
然后我们就可以在 ~/.ideavimrc 中进行vim的配置,配置方法和vim 配置 ~/.vimrc 是一样的

在windows中的文件名略有不同,应该将文件`_ideavimrc`建在`C/Users/当前用户/` 下

## 安装easymotion
使用vim自然离不开使用跳转神器easymotion, 在插件找到`IdeaVim-Easymotion`, 安装后我们可以使用快捷键跳转,文档上有github的地址
默认的跳转比较麻烦,个人比较喜欢nomal 模式下的空格作为搜索跳转的快捷键,为了达到这个效果,我们要使用如下设置
```bash
set easymotion # 必须配这个,文档也有提示,不然启动时默认不开启easymotion
nmap <Space> <Leader><Leader>s # 这里有个注意的点是,要用 nmap 而不是 nnoremap, 否则两个<Leader>键会不生效
```
另外值得注意的是,当使用搜索跳转时,IDE上会使用大写的英文来提示要跳到这个字母要按什么, 但我们并不需要切大写, 比如要跳到`m`, 我们按下`<Leader><Leader>s`,IDE上出现了`A`,但其实我们只要按小写的`a`就可以跳转到目标字符了,用大写的话,还会让内容被选中(因为大写中有shift),这个插件提示可能会让人疑惑并误操作的地方