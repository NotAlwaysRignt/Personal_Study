```bash
let $LANG="zh_CN.UTF-8"
set fileencodings=utf-8,gb2312,gb18030,gbk,ucs-bom,cp936,latin1
set fileencoding=utf-8
set encoding=utf-8

set tags+=./.tags;,.tags
set tags+=~/.cache/tags/
nnoremap <F2> :wa<cr>:AsyncRun ctags -R -f .tags --c++-kinds=+p --fields=+iaS --extra=+q .<CR><CR>
imap jj <Esc> 
set cursorline
highlight CursorLine   cterm=NONE ctermbg=black ctermfg=green guibg=NONE guifg=NONE

set cursorcolumn
highlight CursorColumn cterm=NONE ctermbg=black ctermfg=green guibg=NONE guifg=NONE


set number
syntax on
set mouse=a
set expandtab
set tabstop=4
set shiftwidth=4
set autoindent
set smartindent
set incsearch
set backspace=2

set background=dark
set t_Co=256

"Pathogen 插件管理
execute pathogen#infect()

set nocompatible
filetype on

set rtp+=~/.vim/bundle/vundle/
call vundle#rc()

call plug#begin('~/.vim/plugged')
Plug 'scrooloose/nerdtree'
" ultisips是一个模板自动生成引擎,必须要安装了vim-snippets后才能够运行使用
Plug 'SirVer/ultisnips' 
Plug 'honza/vim-snippets'
Plug 'powerline/powerline'
Plug 'vim-airline/vim-airline'
Plug 'vim-scripts/OmniCppComplete'
Plug 'Yggdroot/LeaderF'
Plug 'ludovicchabant/vim-gutentags'
Plug 'Yggdroot/indentLine'
if has('nvim')
  Plug 'Shougo/deoplete.nvim', { 'do': ':UpdateRemotePlugins' }
else
  Plug 'Shougo/deoplete.nvim'
  Plug 'roxma/nvim-yarp'
  Plug 'roxma/vim-hug-neovim-rpc'
endif
call plug#end()

"deoplete
"安装deoplete后,要再安装一个omnicppcomplete插件,才能在按下. -> 后自动弹窗补全
let g:deoplete#enable_at_startup = 1
let g:deoplete#enable_refresh_always = 1
let g:deoplete#enable_ignore_case = 1
let g:deoplete#enable_smart_case = 1
let g:deoplete#enable_camel_case = 1
let g:deoplete#file#enable_buffer_path = 1
call deoplete#custom#source('_', 'matchers', ['matcher_full_fuzzy'])

call deoplete#custom#option('sources', {
\ '_': ['buffer'],
\ 'cpp': ['buffer', '.tags'],
\})
"
"set omnifunc=syntaxcomplete#Complete
"当设置 noinsert 时,按下tab键后不会自动补全
set completeopt=longest,menuone,noinsert "不要启动preview,就不会在函数补全时弹窗了

call deoplete#custom#option({
\ 'auto_complete':v:true,
\ 'auto_complete_delay': 200,
\ 'smart_case': v:true,
\ 'force_completion_length':3,
\ })


"UliSnips
" Trigger configuration. Do not use <tab> if you use https://github.com/Valloric/YouCompleteMe.
let g:UltiSnipsExpandTrigger="<tab>"

"OmnicppComplete 配合deoplete使用
let OmniCpp_MayCompleteDot = 1 " autocomplete with .
let OmniCpp_MayCompleteArrow = 1 " autocomplete with ->
let OmniCpp_MayCompleteScope = 1 " autocomplete with ::
let OmniCpp_SelectFirstItem = 2 " select first item (but don't insert)
let OmniCpp_NamespaceSearch = 2 " search namespaces in this and included files
let OmniCpp_ShowPrototypeInAbbr = 1 " show function prototype in popup window
let OmniCpp_GlobalScopeSearch=1 " enable the global scope search
let OmniCpp_DisplayMode=1 " Class scope completion mode: always show all members
"let OmniCpp_DefaultNamespaces=["std"]
let OmniCpp_ShowScopeInAbbr=1 " show scope in abbreviation and remove the
let OmniCpp_ShowAccess=1

set rtp+=~/.vim/bundle/SuperTab
let g:SuperTabDefaultCompletionType="context"
"let g:SuperTabDefaultCompletionType="<C-X><C-O>"
let g:SuperTabRetainCompletionType=2
let g:SuperTabCrMapping = 0


Bundle  'scrooloose/nerdtree'
autocmd VimEnter * NERDTree
let NERDTreeQuitOnOpen=0 "打开新标签时也会保持nerdtree窗口
let NERDChristmasTree=1
let NERDTreeAutoCenter=1
let NERDTreeMouseMode=2
let NERDTreeShowBookmarks=1
let NERDTreeShowFiles=1
let NERDTreeShowHidden=1
let NERDTreeWinPos='left'
let NERDTreeWinSize=30
let NERDTreeDirArrows = 0
"
"Bundle 'bling/vim-airline'
set laststatus=2
"
"Bundle 'vim-airline/vim-airline-themes' 
let g:airline_theme="luna" 
"

"Bundle 'Raimondi/delimitMate'
let b:delimitMate_expand_space = 1
" 大括号按下回车键后会有tab
let g:delimitMate_expand_cr = 1 


""ale 语法检查
"let g:ale_linters_explicit = 1   " don't set it
let g:ale_completion_enabled = 1
let g:ale_completion_delay = 500
let g:ale_echo_delay = 20
let g:ale_lint_delay = 500
let g:ale_echo_msg_format = '[%linter%] %code: %%s'
let g:ale_lint_on_text_changed = 'normal'
let g:ale_lint_on_insert_leave = 1
let g:airline#extensions#ale#enabled = 1


let g:gruvbox_contrast_dark='hard'
Bundle 'morhetz/gruvbox'

colorscheme gruvbox


""AsyncRun
""自动打开 quickfix_window,高度为6
let g:asyncrun_open = 6
" 任务结束时候响铃提醒
let g:asyncrun_bell = 1
"" 设置 F10 打开/关闭 Quickfix 窗口
nnoremap <F10> :call asyncrun#quickfix_toggle(6)<cr>
""定义 F9 为编译单文件,这里我用clang而不是gcc
nnoremap <silent> <F9> :w<cr>:AsyncRun gcc -Wall -g "$(VIM_FILEPATH)" -o "$(VIM_FILEDIR)/$(VIM_FILENOEXT)" <cr>
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


"gutengas
" gutentags 搜索工程目录的标志，碰到这些文件/目录名就停止向上一级目录递归
" 找到.root或.git等文件后,自动生成的tags在此根目录下,下面指定的文件必须要创建,否则不会自动生成tags
let g:gutentags_project_root = ['.root', '.svn', '.git', '.hg', '.project']
" 所生成的数据文件的名称
let g:gutentags_ctags_tagfile = '.tags'
" 将自动生成的 tags 文件全部放入 ~/.cache/tags 目录中，避免污染工程目录
"let s:vim_tags = expand('~/.cache/tags')
"let g:gutentags_cache_dir = s:vim_tags
" 配置 ctags 的参数
let g:gutentags_ctags_extra_args = ['--fields=+niazS', '--extra=+q']
let g:gutentags_ctags_extra_args += ['--c++-kinds=+px']
let g:gutentags_ctags_extra_args += ['--c-kinds=+px']


let g:Lf_ShortcutF = '<c-p>'
let g:Lf_ShortcutB = '<m-n>'
noremap <c-n> :LeaderfMru<cr>
noremap <m-p> :LeaderfFunction!<cr>
noremap <m-n> :LeaderfBuffer<cr>
noremap <m-m> :LeaderfTag<cr>
let g:Lf_StlSeparator = { 'left': '', 'right': '', 'font': '' }

let g:Lf_RootMarkers = ['.project', '.root', '.svn', '.git']
let g:Lf_WorkingDirectoryMode = 'Ac'
let g:Lf_WindowHeight = 0.30
let g:Lf_CacheDirectory = expand('~/.vim/cache')
let g:Lf_ShowRelativePath = 0
let g:Lf_HideHelp = 1
let g:Lf_StlColorscheme = 'powerline'
let g:Lf_PreviewResult = {'Function':0, 'BufTag':0}
```
