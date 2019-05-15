### 打印信息
调试在开发过程中是非常重要的一环,最简单的调试就是打印输出
如果使用`python`开发脚本,使用print即可,输出会打印到下方命令窗口上,如果输出很多,在vim中使用`:message`可以查看那过去的输出,这个命令很重要,在查看一些错误历史输出时会很有用

如果不是用`python`而是用`vimscript`开发vim脚本,可以用echo 打印输出,对于多行输出同样可以用`:message`查看

如果信息非常多,建议使用python,因为我们还可以使用其中的日志模块,将信息记录到日志中
### vim 调用 python
#### 直接命令行调用
使用 py3 或 python3 可以执行运行python
```bash
:py3 print("hello world") #如果 vim --version 显示 +python3
# 将在命令窗口打印 hello world
```
导入vim模块
```
py3 import vim # 在vim中执行该导入才会成功
```

#### 运行脚本
当然如果语句很长, 每一句都在 vim 里面输入, 那会非常麻烦,因此我们可以用 vim 文件,将语句都写到 vim 文件里,然后用`:source XXX.vim` 去运行里面的语句
内嵌式执行python
```
py[thon] << {endmarker}
{script}
{endmarker}
```

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
