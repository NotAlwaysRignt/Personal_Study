作者：韦易笑
链接：https://www.zhihu.com/question/47691414/answer/373700711
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
对原文有修改,增添自己的笔记

asyncrun官网:https://github.com/skywind3000/asyncrun.vim  

编译运行再 Vim 8 以前，编译和运行程序要么就让 vim 傻等着结束，不能做其他事情，要么切到一个新的终端下面去单独运行编译命令和执行命令，要么开个 tmux 左右切换。如今新版本的异步模式可以让这个流程更加简化，这里我们使用 AsyncRun 插件，简单设置下：

使用 vim-plugs 安装,在.vimrc处添加:
Plug 'skywind3000/asyncrun.vim'

或使用 vundle 安装,在.vimrc处添加:
Plugin 'skywind3000/asyncrun.vim'
然后运行:PluginInstall

```vim
" 自动打开 quickfix window ，高度为 6
let g:asyncrun_open = 6

" 任务结束时候响铃提醒
let g:asyncrun_bell = 1

" 设置 F10 打开/关闭 Quickfix 窗口
nnoremap &lt;F10&gt; :call asyncrun#quickfix_toggle(6)&lt;cr&gt;  
```

该插件可以在后台运行 shell 命令，并且把结果输出到 quickfix 窗口：  
<img src="https://pic2.zhimg.com/50/v2-b683e6b77a7808fa31e9d4cbe2177104_hd.jpg" data-caption="" data-size="normal" data-rawwidth="680" data-rawheight="460" data-thumbnail="https://pic2.zhimg.com/50/v2-b683e6b77a7808fa31e9d4cbe2177104_hd.jpg" class="origin_image zh-lightbox-thumb" width="680" data-original="https://pic2.zhimg.com/v2-b683e6b77a7808fa31e9d4cbe2177104_r.jpg">  
最简单的编译单个文件，和 sublime 的默认 build system 差不多，我们定义 F9 为编译单文件:
```vim
nnoremap &lt;silent&gt; &lt;F9&gt; :AsyncRun gcc -Wall -O2 "$(VIM\_FILEPATH)" -o "$(VIM\_FILEDIR)/$(VIM\_FILENOEXT)" &lt;cr&gt;
```
**上面是使用gcc编译,如果想用clang,则把上面的gcc替换为clang,另外在按F9编译前我们得先用 :w保存文件**
其中 $(...) 形式的宏在执行时会被替换成实际的文件名或者文件目录，这样按 F9 就可以编译当前文件，同时按 F5 运行：
```vim
nnoremap &lt;silent&gt; &lt;F5&gt; :AsyncRun -raw -cwd=$(VIM\_FILEDIR) "$(VIM\_FILEDIR)/$(VIM_FILENOEXT)" &lt;cr&gt;
```

用双引号引起来避免文件名包含空格，“-cwd=$(VIM\_FILEDIR)” 的意思时在文件文件的所在目录运行可执行，后面可执行使用了全路径，避免 linux 下面当前路径加 “./” 而 windows 不需要的跨平台问题。  
参数 `-raw` 表示输出不用匹配错误检测模板 (errorformat) ，直接原始内容输出到 quickfix 窗口。这样你可以一边编辑一边 F9 编译，出错了可以在 quickfix 窗口中按回车直接跳转到错误的位置，编译正确就接着执行。  
接下来是项目的编译，不管你直接使用 make 还是 cmake，都是对一群文件做点什么，都需要定位到文件所属项目的目录，AsyncRun 识别当前文件的项目目录方式和 gutentags相同，从文件所在目录向上递归，直到找到名为 “.git”, “.svn”, “.hg”或者 “.root”文件或者目录，如果递归到根目录还没找到，那么文件所在目录就被当作项目目录，你重新定义项目标志：
```vim
let g:asyncrun\_rootmarks = ['.svn', '.git', '.root', '_darcs', 'build.xml'] 
```  

**注意,下面的编译整个项目,运行,以及测试本质都是将make 映射到快捷键上,所以前提是写好了相应的makefile文件**
然后在 AsyncRun 命令行中，用 “&lt;root&gt;” 或者 “$(VIM_ROOT)”来表示项目所在路径，于是我们可以定义按 F7 编译整个项目：  
```vim
nnoremap &lt;silent&gt; &lt;F7&gt; :AsyncRun -cwd=&lt;root&gt; make &lt;cr&gt;
```


那么如果你有一个项目不在 svn 也不在 git 中怎么查找 &lt;root&gt; 呢？很简单，放一个空的 .root 文件到你的项目目录下就行了，前面配置过，识别名为 .root 的文件。继续配置用 F8 运行当前项目：
```vim
nnoremap &lt;silent&gt; &lt;F8&gt; :AsyncRun -cwd=&lt;root&gt; -raw make run &lt;cr&gt;
```

当然，你的 makefile 中需要定义怎么 run ，接着按 F6 执行测试：
```vim
nnoremap &lt;silent&gt; &lt;F6&gt; :AsyncRun -cwd=&lt;root&gt; -raw make test &lt;cr&gt;
```
如果你使用了 cmake 的话，还可以照葫芦画瓢，定义 F4 为更新 Makefile 文件，如果不用 cmake 可以忽略：
```vim
nnoremap &lt;silent&gt; &lt;F4&gt; :AsyncRun -cwd=&lt;root&gt; cmake . &lt;cr&gt;
```
由于 C/C++ 标准库的实现方式是发现在后台运行时会缓存标准输出直到程序退出，你想实时看到 printf 输出的话需要 fflush(stdout) 一下，或者程序开头关闭缓存：“setbuf(stdout, NULL);” 即可。  
同时，如果你开发 C++ 程序使用 std::cout 的话，后面直接加一个 std::endl 就强制刷新缓存了，不需要弄其他。而如果你在 Windows 下使用 GVim 的话，可以弹出新的 cmd.exe 窗口来运行刚才的程序：  
```vim
nnoremap &lt;silent&gt; &lt;F5&gt; :AsyncRun -cwd=$(VIM\_FILEDIR) -mode=4 "$(VIM\_FILEDIR)/$(VIM_FILENOEXT)" &lt;cr&gt;  
nnoremap &lt;silent&gt; &lt;F8&gt; :AsyncRun -cwd=&lt;root&gt; -mode=4 make run &lt;cr&gt;  
```
在 Windows 下使用 -mode=4 选项可以跟 Visual Studio 执行命令行工具一样，弹出一个新的 cmd.exe窗口来运行程序或者项目，于是我们有了下面的快捷键：  
F4：使用 cmake 生成 Makefile   
F5：单文件：运行  
F6：项目：测试  
F7：项目：编译  
F8：项目：运行  
F9：单文件：编译  
F10：打开/关闭底部的 quickfix 窗口  
编译和运行基本和 NotePad++ / GEdit 的体验差不多了。如果你重度使用 cmake 的话，你还可以写点小脚本，将 F4 和 F7 的功能合并，检测 CMakeLists.txt 文件改变的话先执行 cmake 更新一下 Makefile，然后再执行 make，否则直接执行 make，这样更自动化些。
