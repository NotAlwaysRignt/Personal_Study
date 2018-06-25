```vim
syntax on  "语法高亮

set fileencodings=utf-8,ucs-bom,gb18030,gbk,gb2312,cp936
set termencoding=utf-8
set encoding=utf-8

set ts=4   "TAB = 4 space
set expandtab

set shiftwidth=4

set cursorline "突出光标所在行

"vim可用鼠标改变光标位置
set mouse=a

"每5秒自动保存
let autosave = 5

"开启行号显示
set number

"搜索模式时,输入字符串时就高亮查找
set incsearch

"在屏幕行中跳转,而不是以回车为界
nnoremap j gj
nnoremap k gk
nnoremap ^ g^
nnoremap $ g$


"插入模式中用jj替代Esc,快速按下
imap jj <Esc>

"常用符号的自动补全,注意不建议给尖括号提供自动补全,否则影响C++或HTML的补全功能
inoremap ' ''<Esc>i
inoremap " ""<Esc>i
inoremap ( ()<Esc>i
inoremap { {}<Esc>i

" Pathogen load
filetype off
call pathogen#infect()
call pathogen#helptags()
filetype plugin indent on


"start pydiction
filetype plugin on
let g:pydiction_location = '~/.vim/bundle/pydiction/complete-dict'



let g:pymode_python = 'python3'
let g:pymode = 1
let g:pymode_warnings = 1
let g:pymode_paths = []
let g:pymode_trim_whitespaces = 1
let g:pymode_folding = 1


let g:pymode_rope_completion = 0
let g:pymode_rope_complete_on_dot = 0

"Turn on the run code script
let g:pymode_run = 1

"Binds keys to run python code
let g:pymode_run_bind = '<leader>r'



"By default when you press <C-C>g on any object in your code you will be moved
"to definition.
let g:pymode_rope_goto_definition_bind = '<C-c>g'

"Keymap for rename method/function/class/variables under cursor
let g:pymode_rope_rename_bind = '<C-c>rr'

"Turn on pymode syntax
let g:pymode_syntax = 1

"Enable all python highlights 
let g:pymode_syntax_all = 1



"vundle
set nocompatible              " be iMproved, required
filetype off                  " required

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin('~/.vim/bundle/vundle/')

" let Vundle manage Vundle, required
Plugin 'VundleVim/Vundle.vim'

"asyncrun
Plugin 'skywind3000/asyncrun.vim'

"YouComplteMe
Plugin 'Valloric/YouCompleteMe'

"YCM-Generator
Plugin 'rdnetto/YCM-Generator'

Plugin 'jistr/vim-nerdtree-tabs'
call vundle#end()            " required
filetype plugin indent on    " required

"NERDTree
" 在终端启动vim时，共享NERDTree
let g:nerdtree_tabs_open_on_console_startup=1

"YouCompleteMe
let g:ycm_server_keep_logfiles = 1 
let g:ycm_server_log_level = 'debug'
let g:ycm_global_ycm_extra_conf='~/.vim/bundle/vundle/YouCompleteMe/.ycm_extra_conf.py'
let g:ycm_semantic_triggers =  {
			\ 'c,cpp,python,java,go,erlang,perl': ['re!\w{2}'],
			\ 'cs,lua,javascript': ['re!\w{2}'],
			\ }
set completeopt=menu,menuone
let g:ycm_add_preview_to_completeopt = 0

"跳转到定义或声明快捷键
map gd :YcmCompleter GoToDefinitionElseDeclaration<CR>

"asyncrun配置

" 自动打开 quickfix window ，高度为 6
let g:asyncrun_open = 6
" 任务结束时候响铃提醒
let g:asyncrun_bell = 1
" 设置 F10 打开/关闭 Quickfix 窗口
nnoremap <F10> :call asyncrun#quickfix_toggle(6)<cr>
"定义 F9 为编译单文件,这里我用clang而不是gcc
nnoremap <silent> <F9> :w<cr>:AsyncRun clang++ -Wall -g -std=c++11 "$(VIM_FILEPATH)" -o "$(VIM_FILEDIR)/$(VIM_FILENOEXT)" <cr>
" F5运行
nnoremap <silent> <F5> :AsyncRun -raw -cwd=$(VIM_FILEDIR) "$(VIM_FILEDIR)/$(VIM_FILENOEXT)" <cr>

" 如何确定根目录
let g:asyncrun_rootmarks = ['.svn', '.git', '.root', '_darcs', 'build.xml'] 
" F7 执行 make 编译整个项目
nnoremap <silent> <F7> :wa<cr> :AsyncRun -cwd=<root> make <cr>
"F8 执行 make run 运行当前项目 
nnoremap <silent> <F8> :AsyncRun -cwd=<root> -raw make run <cr>
" F6 执行make test测试当前项目
nnoremap <silent> <F6> :AsyncRun -cwd=<root> -raw make test <cr>



"快速生成C++的main函数模板
inoremap !C #include<iostream><Esc>o#include<string><Esc>o#include<vector><Esc>o#include<map><Esc>ousing namespace std;<Esc>o<Esc>oint main(){}<Esc>i<CR><Esc>Oreturn 0;<Esc>O


"solarized background theme
syntax enable
let g:solarized_termcolors=256
set background=dark
colorscheme solarized
```
