参考资料:
https://cloud.tencent.com/developer/article/1172325
https://github.com/hy172574895/YCMAnalysis

### YCM 是怎么启动的
已知 vim 在启动时会去运行 plugin/*.vim 下的插件,因此我们可以从 plugin 目录下开始看
下载 YCM 的项目目录,在`plugin/youcompleteme.vim`下可以看到
```vim
if has( 'vim_starting' ) " Loading at startup.
  " We defer loading until after VimEnter to allow the gui to fork (see
  " `:h gui-fork`) and avoid a deadlock situation, as explained here:
  " https://github.com/Valloric/YouCompleteMe/pull/2473#issuecomment-267716136
  augroup youcompletemeStart
    autocmd!
    autocmd VimEnter * call youcompleteme#Enable()
  augroup END 
else " Manual loading with :packadd.
  call youcompleteme#Enable()
endif
```
根据 stackoverflow: https://stackoverflow.com/questions/50428364/add-hasvim-starting-in-the-start-of-vimrc
> has("vim_starting") returns true on startup, false when running

vim 在刚启动时has("vim_starting")为true,运行时为false,因此,启动vim时,`call youcompleteme#Enable()`将对 YCM 进行初始化

YCM 的`youcompleteme#Enable()`是 YCM 的入口,这个`vim`函数的定义在 `autoload/youcompleteme.vim` 中因此当我们想要阅读 YCM 时,只要从此函数开始即可

### YCM 架构简析
YCM 采用 server-client 模式, YCM 启动时,会通过 Python 去启动一个 server 进程和一个 client 进程,我们在 vim 中输入的时候,buffer 里的信息会作为 client 的发送内容发给 server (以 json 的格式)
server 端收到信息后,再调用 clang 库来解析这些语法,解析完毕后将结果发回给 client(用 gcc编译好,然后通过 python调用 c 的形式去调用它)#这个 CMakeLists.txt 执行的主要功能: 通过 FILE(download) 到clang 官网下载相应版本的 clang,然后最终编译生成ycm_core文件 (ycm_core.pyd,也可能是ycm_core.so,ycm_core.dll),阅读这个CMakeLists,看 project函数,知道 PROJECT_NAME,然后看 add_library,这是最终生成目标,看set_target_properties,这是编译输出的属性


### 安装 YCM 的难点
YCM 之所以难以安装,是因为需要先去远端下载一个 clang 库并进行编译,这些操作都统一集成到 build.py 里去了,编译过程中稍有差错就会失败,因为整个过程很复杂,所以出错了也难以调试
build.py 首先干的是通过cmake 执行`third_party/ycmd/cpp/CMakeLists.txt`,这个文件首先会做一些操作系统等的相关检查,以及一些变量设置,接着最后调用`add_subdirectory( ycm )`,即用 cmake 执行`./third_party/ycmd/cpp/ycm/CMakeLists.txt`,这个文件是下载编译 clang 库的核心所在
它主要完成的任务: 通过 FILE(download) 到clang 官网下载相应版本的 clang,然后最终编译生成`ycm_core`文件 (`ycm_core.pyd`,也可能是`ycm_core.so`,`ycm_core.dll`)
阅读这个 CMakeLists 的思路,通过 `project` 函数,可以知道 `PROJECT_NAME`,然后看 `add_library`了解最终这个`CMakeLists`最终要生成的目标,`set_target_properties`设置了编译时的属性,如用编译时用的flag,输出库的名字等
而且这是一个很好的 cmake demo,如果要学习cmake,建议阅读

我们可以在`./third_party/ycmd/cpp/ycm/CMakeLists.txt`中看到,它会通过网络远程去下载 clang库,通过`file(DOWNLOAD)`这个功能,因为 clang 库的版本是被指定的,所以如果 clang 库版本等级过高和操作系统不兼容也会出错
```CMakeLists.txt
  if( LIBCLANG_DOWNLOAD )
    message( STATUS
             "Downloading libclang ${CLANG_VERSION} from ${LIBCLANG_URL}" )

    file(
        DOWNLOAD "${LIBCLANG_URL}" "${LIBCLANG_LOCAL_FILE}" #这里会去 LIBCALNG_URL里下载 clang,
      SHOW_PROGRESS EXPECTED_HASH SHA256=${LIBCLANG_SHA256}
    )
  else()
    message( STATUS "Using libclang archive: ${LIBCLANG_LOCAL_FILE}" )
  endif()
```

### YCM python 代码组织
我们从`autoload/youcompleteme.vim`的`youcompleteme#Enable()`开始,这个是 vim 才启动时(has('vim_starting') )调用的函数,python代码的组织是在`s:SetUpPython()`里完成的

先看看 `youcompleteme#Enable()`,先初始化各种设置,然后通过`autocmd`定义了各种事件对应的回调函数
```shell
function! youcompleteme#Enable()
  call s:SetUpBackwardsCompatibility()

  if !s:SetUpPython()
    return
  endif

  call s:SetUpOptions()

  call youcompleteme#EnableCursorMovedAutocommands()
  augroup youcompleteme
    autocmd!
    " Note that these events will NOT trigger for the file vim is started with;
    " so if you do "vim foo.cc", these events will not trigger when that buffer
    " is read. This is because youcompleteme#Enable() is called on VimEnter and
    " that happens *after* FileType has already triggered for the initial file.
    " We don't parse the buffer on the BufRead event since it would only be
    " useful if the buffer filetype is set (we ignore the buffer if there is no
    " filetype) and if so, the FileType event has triggered before and thus the
    " buffer is already parsed.
    autocmd FileType * call s:OnFileTypeSet()
    autocmd BufEnter * call s:OnBufferEnter()
    autocmd BufUnload * call s:OnBufferUnload()
    autocmd InsertLeave * call s:OnInsertLeave()
    autocmd VimLeave * call s:OnVimLeave()
    autocmd CompleteDone * call s:OnCompleteDone()
    autocmd BufEnter,WinEnter * call s:UpdateMatches()
  augroup END

  " The FileType event is not triggered for the first loaded file. We wait until
  " the server is ready to manually run the s:OnFileTypeSet function.
  let s:pollers.server_ready.id = timer_start(
        \ s:pollers.server_ready.wait_milliseconds,
        \ function( 's:third_party/ycmd/ycmd/server_utils.pyPollServerReady' ) )
endfunction
```

通过`s:SetUpPython()`我们就可以知道它是如何组织这个项目了,这个函数的主要功能是将整个 YouCompleteMe 项目的python代码所在目录都添加到 sys.path 中,这样就可以对需要的模块进行 import 了
```python
function! s:SetUpPython() abort
  exec s:python_until_eof
from __future__ import unicode_literals
from __future__ import print_function
from __future__ import division
from __future__ import absolute_import

import os.path as p
import sys
import traceback
import vim

root_folder = p.normpath( p.join( vim.eval( 's:script_folder_path' ), '..' ) )
third_party_folder = p.join( root_folder, 'third_party' )
ycmd_third_party_folder = p.join( third_party_folder, 'ycmd', 'third_party' )

# Add dependencies to Python path.
dependencies = [ p.join( root_folder, 'python' ),
                 p.join( third_party_folder, 'requests-futures' ),
                 p.join( third_party_folder, 'ycmd' ),
                 p.join( ycmd_third_party_folder, 'frozendict' ),
                 p.join( ycmd_third_party_folder, 'requests' ) ]

# The concurrent.futures module is part of the standard library on Python 3.
if sys.version_info[ 0 ] == 2:
  dependencies.append( p.join( third_party_folder, 'pythonfutures' ) )

sys.path[ 0:0 ] = dependencies #将dependencies的路径都添加到 sys.path 中,sys.path[0:0]会使这些路径添加到列表的最前面

# We enclose this code in a try/except block to avoid backtraces in Vim.
try:
  # The python-future module must be inserted after the standard library path.
  from ycmd.server_utils import GetStandardLibraryIndexInSysPath
  sys.path.insert( GetStandardLibraryIndexInSysPath() + 1,
                   p.join( ycmd_third_party_folder, 'python-future', 'src' ) )

  # Import the modules used in this file.
  from ycm import base, vimsupport, youcompleteme

  ycm_state = youcompleteme.YouCompleteMe()
except Exception as error:
  # We don't use PostVimMessage or EchoText from the vimsupport module because
  # importing this module may fail.
  vim.command( 'redraw | echohl WarningMsg' )
  for line in traceback.format_exc().splitlines():
    vim.command( "echom '{0}'".format( line.replace( "'", "''" ) ) )

  vim.command( "echo 'YouCompleteMe unavailable: {0}'"
               .format( str( error ).replace( "'", "''" ) ) )
  vim.command( 'echohl None' )
  vim.command( 'return 0' )
else:
  vim.command( 'return 1' )
EOF
endfunction
```


我们在`third_party/ycmd/ycmd/server_utils.py`可以看见编译好的 clang 库被 python 加载
```python
def ImportCore():
  """Imports and returns the ycm_core module. This function exists for easily
  mocking this import in tests."""
  import ycm_core as ycm_core
  return ycm_core

def CompatibleWithCurrentCore():
  """Checks if ycm_core library is compatible and returns with an exit
  status."""
  try:
    ycm_core = ImportCore()
  except ImportError as error:
    # ...

  try:
    current_core_version = ycm_core.YcmCoreVersion()
  # ...
```
通过前面知道,`CMakeLists.txt`最后编译产生的目标文件就是`ycm_core.pyd`(也可能是.so,.dll作为后缀)
而以".py"、".pyo"、".pyc"、".pyd"、".so"、".dll"为后缀的文件都可以被python作为 module 导入
因此,正是通过`import ycm_core as ycm_core` 这句实现了 clang 库是被 python 加载

观察`current_core_version = ycm_core.YcmCoreVersion()`一句,用 grep 可以查得`YcmCoreVersion()`并没有定义在一个py文件里
而是声明在`./third_party/ycmd/cpp/ycm/versioning.h`且定义在`./third_party/ycmd/cpp/ycm/versioning.cpp`这两个文件里,可见这里已经是 python 对 C++ 的调用
