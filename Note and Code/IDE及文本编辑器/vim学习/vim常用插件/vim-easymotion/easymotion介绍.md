### vim-easymotion 介绍与实践应用
[vim-easymotion官网](https://github.com/easymotion/vim-easymotion)
vim-easymotion 是用纯vim脚本编写的一个插件,其核心功能是实现光标的快速跳转
相较于vim 本身的 w,e,f 等跳转方式, vim-easymotion 提供了跨行跳转的功能,本人非常推荐使用这个功能,它使得在当前页面下跳转操作变得更快捷,熟练运用,即可实现看哪跳转哪的操作

具体的跳转方式的支持有很多,个人建议只需要使用一个功能`<Leader><Leader>s`即可,它是跨行`f`和`F`的结合,我们可以快速地跳转到我们想要的字母处,为了更快速地实现跳转,建议采取如下配置
```bash
map <Space> <Leader><Leader>s
```
`map`在 Normal, Visual, Select 模式下均会生效,但在 Insert 模式下不会,这样我们既可以在 Normal 模式下快速跳转到想要到达的位置,又可以在 Visual 或 Select 模式下快速选取范围
