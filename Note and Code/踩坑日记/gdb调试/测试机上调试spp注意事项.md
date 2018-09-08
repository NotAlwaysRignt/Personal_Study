### 修改配置
设置 pronumc 为1，即worker数为1，这样才能确保挂住的worker就是处理请求的worker，如果有多个worker，就不能确保某个请求一定被某个worker处理

#### 进程名字
当使用 sh spp.sh start 启动spp 服务的时候，会根据 ../etc/spp_ctrl.xml 的 exe 属性去寻找当前目录下的可执行文件 worker proxy ctrl
它们的名字会在 ps 命令中显示出来，因此我们往往要改变这些可执行文件的名字，同时修改配置文件，这样我们才能很方便地用 ps 命令找到它们
```bash
# 寻找名字为 chaoranxu 的进程
ps -ef | grep chaoranxu
```

#### gdb调试
##### gdb挂住 Ctrl
我们一旦用gdb挂住worker，ctrl就会认为worker 挂了，于是就杀死它并重启，这样我们就调试不了了，所以要先挂住 ctrl 进程，挂住的方法是，通过 ps 命令找到 ctrl 的进程号, 然后用
```
# 假设 ctrl 的进程号为17577
gdb --pid 17577
```
挂住 ctrl 进程，如果我们挂住后按c，ctrl又会继续执行

##### gdb 挂住worker
这一步非常重要  
**我们要给代码打断点，一定要在用 gdb --pid 挂住worker 后在这个worker 的gdb里打断点，在ctrl中是找不到的**
在使用gdb 挂住 worker时，**请确保使用gdb命令是在存放 spp.sh 的 bin 目录下**，另外要给代码打断点，还要**将要打断点的源代码文件存放到当前目录下**，请确保编译生成的so文件和源代码是对应的，即不能用以前版本的代码和当前版本的so，这样打断点时查看代码会出错

另外，如果你发现使用进入gdb 交互界面后还是打不了断点，请观察你的目录结构，比如你的代码存放在src 目录下，不妨试试
```bash
b src/my.cpp：断点号
```
这样打断点的文件了，如果打断点时显示没有找到源文件，那肯定是你操作出错了

#### 可能出现的问题
* 如果 worker 调试过程中挂了，切换到挂住 ctrl 的 gdb 界面，按下 c，这时候 worker 会被重启，接下来按 ctrl c 又可以把 ctrl 挂住，用ps 找到worker进程 挂住它调试， 这样就免去了重启整个spp服务的麻烦
* gdb可能会导致进程挂掉，比如worker，有时候 ctrl 也会不小心弄挂，然后使用 sh spp.sh stop 后发现 proxy 没关掉，而测试机上又没有 kill 的权限，我们可以用gdb 挂住 proxy，然后按c，再按 ctrl-c ，然后使用quit 退出命令，这样可以杀死这个进程，相当于kill的效果，一次不行，多试几次