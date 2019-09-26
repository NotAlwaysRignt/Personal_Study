#!/usr/bin/env python
#coding=utf-8

import sys, os
import logging

# The test function for VIM plugin
def my_test(vim, vim_ver):
    buf = vim.current.buffer
    # append process id & python version info to the buffer
    #buf.append("VIM process %d; vim version: %s\n" % (os.getpid(), vim_ver))
    buf.append("VIM process %d; vim version: %s\n" % (os.getpid(), vim_ver))
    logging.basicConfig(level=logging.DEBUG,  
                        format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',  
                        datefmt='%a, %d %b %Y %H:%M:%S',  
                        filename='./test.log',  
                        filemode='w') 
    logging.debug("hello world!")


#使用虚拟环境
def use_virtual():
    if 'VIRTUAL_ENV' in os.environ:
        project_base_dir = os.environ['VIRTUAL_ENV']
        #虚拟环境下执行activate_this.py会启动虚拟环境
        activate_this = os.path.join(project_base_dir, 'bin/activate_this.py')
        exec(open(activate_this).read(),dict(__file__=activate_this))
        print(os.environ['PATH'])
        import requests #引入虚拟环境安装的requests
        print(requests.get('https://api.github.com/events'))

if __name__ == "__main__":
    use_virtual()

