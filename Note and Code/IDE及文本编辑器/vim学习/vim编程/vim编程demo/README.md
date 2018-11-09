### demo
vim 打开 add.txt, 输入`source my_vim_plugin.vim`,接着`:call MyTestFunc()`便会执行 py脚本中的内容

### vim 调用python
#### 直接命令行调用
使用 py3 或 python3 可以执行运行python
```bash
:py3 print("hello world") #如果 vim --version 显示 +python3
# 将在命令窗口打印 hello world
```

```
py3 import vim # 在vim中执行该导入才会成功
```

#### 脚本调试
当然如果语句很长, 每一句都在 vim 里面输入, 那会非常麻烦,因此我们可以用 vim 文件,将语句都写到 vim 文件里,然后用`:source XXX.vim` 去运行里面的语句
内嵌式执行python
```
py[thon] << {endmarker}
{script}
{endmarker}
```

### 调试
开发最重要的是调试,事实上,使用 vim 脚本调试并不简单. 一个建议的方式是用 python 开发vim脚本,这样我们就可以通过调用 python 打日志,单元测试等手段来进行调试

### 虚拟环境
默认情况下 vim 启动的python 并不知道你的虚拟环境,但使用虚拟环境往往是有必要的,下面的python代码可以帮助启动 虚拟环境
```python
"python with virtualenv support
py << EOF
import os
import sys
#启动虚拟环境后,VIRTUAL_ENV 会被赋值
if 'VIRTUAL_ENV' in os.environ:
    project_base_dir = os.environ['VIRTUAL_ENV']
    #虚拟环境下执行activate_this.py会启动虚拟环境
    activate_this = os.path.join(project_base_dir, 'bin/activate_this.py')
    exec(open(activate_this).read(),dict(__file__=activate_this)) #py3中使用 exec 而不是execfile
EOF
```
