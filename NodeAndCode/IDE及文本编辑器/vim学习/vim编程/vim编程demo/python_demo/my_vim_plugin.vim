" A simple VIM plugin for test the python module in VIM.
" 
" Loading the plugin: 
" 1. Save the my-vim-plugin.py & my-vim-plugin.vim in the same folder.
" 2. Open the my-vim-plugin.vim in your VIM.
" 3. run "so %" 
"
" After loading the plugin, you can call the "MyTestFunc()" in vim.
" This test function will append some python info to the your vim
" buffer.
"

" Load this module only once.
if exists('loaded_mytestplugin')
    finish
endif

" Check vim python runtime
if !has('python3')
    echo "Error: Required vim compiled with +python"
    finish
endif

" Check vim version
if v:version < 700
    echo "Error: Required vim version +7"
    finish
endif

" Set the loaded flag
let loaded_mytestplugin = 1

" Add my plugin to the path
python3 import sys, vim
python3 sys.path.append(vim.eval('expand("<sfile>:h")'))

" my test function
function! MyTestFunc()
python3 << endOfPython

# import vim object. Regarding the detail of the vim object,
# please check the ":help if_pyth.txt" in vim document.
import vim

# call our python function
import my_vim_plugin as myVimPlugin
myVimPlugin.my_test(vim, vim.eval("v:version"))

endOfPython
endfunction
function! MyEnv()
python3 << endOfPython
import my_vim_plugin as myVimPlugin
myVimPlugin.use_virtual()
endOfPython
endfunction
call MyEnv()
