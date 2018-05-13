### 非常有必要的配置  
#### 输入字母时自动补全
默认的 YCM 配置并不完美,比如它只在输入'.','->','::' 才会进行语义分析,比如我们输入 printf,它是不会提示的,除非我们以前输入过,但输入过才会提示显然也不能让我们满意,解决这个问题,可以添加以下配置
```bash
let g:ycm_semantic_triggers =  {
            \ 'c,cpp,python,java,go,erlang,perl': ['re!\w{2}'],
            \ 'cs,lua,javascript': ['re!\w{2}'],
            \ }
```
['re!\w{2}'] 的意思是,使用正则表达式,当我们输入两个连续的字母时,就会启动补全提示  

#### 关闭预览窗口
另外还有一个很不喜欢的功能,就是补全时还会弹出一些预览窗口,说明函数功能,这些窗口很多时候不会有有用的信息,禁用掉它可以设置  
```bash  
set completeopt=menu,menuone
let g:ycm_add_preview_to_completeopt = 0
```
关于一些常用配置可以参考这篇文章:  
https://zhuanlan.zhihu.com/p/33046090  

#### 跳转到定义或声明
YCM 自带了跳转到定义或声明的功能,使用方法,输入:
:YcmCompleter GoToDefinition (跳转到定义)  
:YcmCompleter GoToDeclaration (跳转到声明)  
YcmCompleter GoToDefinitionElseDeclaration  (跳转到定义,不能跳转则跳转到声明)  
**上面的跳转除了可以进入函数或变量的定义或声明以外,YCM 也支持文件的跳转,当光标在 include 里的文件名上时执行跳转就可进入文件,非常强大!**  
虽然可以在输入时用 Tab补全,但是还是不方便.所以建议使用快捷键映射
  
map gd :YcmCompleter GoToDefinitionElseDeclaration&lt;CR&gt;  
即把 :YcmCompleter GoToDefinitionElseDeclaration 映射到快捷键 gd,这样可以很方便地查看函数
当跳转到文件时,如果想跳转回来  
ctrl o (跳转回前一个位置)

### 其它参考配置
```bash
" 自动补全配置
set completeopt=longest,menu    "让Vim的补全菜单行为与一般IDE一致(参考VimTip1228)
autocmd InsertLeave * if pumvisible() == 0|pclose|endif "离开插入模式后自动关闭预览窗口
inoremap <expr> <CR>       pumvisible() ? "\<C-y>" : "\<CR>"    "回车即选中当前项
"上下左右键的行为 会显示其他信息
inoremap <expr> <Down>     pumvisible() ? "\<C-n>" : "\<Down>"
inoremap <expr> <Up>       pumvisible() ? "\<C-p>" : "\<Up>"
inoremap <expr> <PageDown> pumvisible() ? "\<PageDown>\<C-p>\<C-n>" : "\<PageDown>"
inoremap <expr> <PageUp>   pumvisible() ? "\<PageUp>\<C-p>\<C-n>" : "\<PageUp>"

"youcompleteme  默认tab  s-tab 和自动补全冲突
"let g:ycm_key_list_select_completion=['<c-n>']
let g:ycm_key_list_select_completion = ['<Down>']
"let g:ycm_key_list_previous_completion=['<c-p>']
let g:ycm_key_list_previous_completion = ['<Up>']
let g:ycm_confirm_extra_conf=0 "关闭加载.ycm_extra_conf.py提示

let g:ycm_collect_identifiers_from_tags_files=1 " 开启 YCM 基于标签引擎
let g:ycm_min_num_of_chars_for_completion=2 " 从第2个键入字符就开始罗列匹配项
let g:ycm_cache_omnifunc=0  " 禁止缓存匹配项,每次都重新生成匹配项
let g:ycm_seed_identifiers_with_syntax=1    " 语法关键字补全
nnoremap <F5> :YcmForceCompileAndDiagnostics<CR>    "force recomile with syntastic
"nnoremap <leader>lo :lopen<CR> "open locationlist
"nnoremap <leader>lc :lclose<CR>    "close locationlist
inoremap <leader><leader> <C-x><C-o>

"在注释输入中也能补全
let g:ycm_complete_in_comments = 1
"在字符串输入中也能补全
let g:ycm_complete_in_strings = 1
"注释和字符串中的文字也会被收入补全
let g:ycm_collect_identifiers_from_comments_and_strings = 0

nnoremap <leader>jd :YcmCompleter GoToDefinitionElseDeclaration<CR> " 跳转到定义处
```
