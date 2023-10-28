### Leaderf 介绍
Leaderf 是由python编写的vim插件,它是模糊搜索的工具,搜索的内容包括文件,函数,tag 等,并且是异步的,不会导致 vim 阻塞,是异步 vim 时代下 ctrlp 的替代品

#### 常用功能介绍
Leaderf 支持许多内容的搜索,个人认为有必要掌握的常用功能有
* 查找文件,从当前根目录下开始查找,支持文件名模糊匹配的功能,它的搜索速度极快,对应命令`:LeaderfFile`,也可以通过`g:Lf_ShortcutF`设置快捷键,默认为`<Leader>f`
* 查找最近打开的文件(Mru),使用vim最近打开的文件都会显示于列表中,对应命令`:LeaderfMru`
* 函数列表,显示当前文件夹下的所有函数,支持函数快速跳转,对应命令`:LeaderfFunction`
* tag查找,使用 ctags 生成 tag 以后,可以根据 tag 的内容快速搜索变量的位置,在阅读源码时非常方便,对应命令`:LeaderfTag`

#### 打开窗口
Leaderf 提供的快捷键很多,个人认为最有必要掌握的有以下几个:
* 在新的tag页打开,对应默认快捷键`<C-t>`(t大小写均可)
* 垂直切分窗口打开,对应默认快捷键`<C-]>`
以上快捷键在搜寻文件并阅读其内容时很有用,不会覆盖原来的窗口

#### 个人配置
```bash
nmap <Leader>m :LeaderfMru<cr>
nmap <Leader>F :LeaderfFunction<cr>
let g:Lf_ShortcutF= '<Leader>f' " :LeaderfFile 
noremap <Leader>t :LeaderfTag<cr>  
"常用快捷键:<C-T>在新的tab打开显示文件,<C-]>在新的垂直窗口打开
let g:Lf_StlSeparator = { 'left': '', 'right': '', 'font': '' }
```
