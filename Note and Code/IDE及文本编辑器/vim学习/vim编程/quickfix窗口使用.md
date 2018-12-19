vim 中 quickfix 是一个窗口,这个窗口特殊之处在于,它不仅可以输出信息,还可以支持一些更高级的操作,比如当编译器输出错误信息时,可以根据输出信息指定的文件来进行代码跳转
quickfix窗口 早期是 vim 中的一个 插件,现在已经集成到了 vim 中.学会使用它,我们可以用来写一些自己的插件

**要详细了解 quick-fix 窗口的各个函数及命令可以输入`:help quickfix-window`查看vim 内置的文档,要注意的是通过`:help quickfix-window`查看到的文档并不是全部,有些函数比如setqflist会给出例子,不过函数具体调用要用`:help setqflist`去进一步查看那** 下面只介绍 quickfix 窗口常用功能
### 打开 quickfix 窗口
开启quickfix 窗口的命令 `copen`, 可以查看在vim 中查看文档`:help copen`
在使用脚本编程时我们可以使用 exec 来执行这个命令,进而开启窗口
```shell
exec 'copen'
```
copen前后还可以加一些附带操作,比如我们希望窗口启动在特殊的位置,可以使用`botright copen` 或 `bot copen`,可以用`help :bot`查看
我们还希望窗口打开后能指定它的高度,可以在 copen 后跟参数(不跟参数的话会有默认值)
```shell
botright copen 10
```

### 操作 quickfix 窗口
要在 quickfix 窗口中添加内容,可以使用函数`setqflist`,具体的使用可以通过`help setqflist()`查看使用方法
另外`:help quickfix-window`也介绍了 `setqflist` 的例子,可以查看一下

setqflist 的函数形式如下
```shell
setqflist({list} [, {action} [, {what}]])
```

举例,先用`:copen`打开一个quickfix窗口,接着
```shell
:call setqflist([{'text':"hello wrold"}],'a')
```
可以看见quickfix窗口追加了一行`hello world`
参数 a 是一种模式表示追加,具体见文档
要清除 quickfix 窗口的所有内容,可以调用`:call setqflist([], 'r')`,r 是一种模式,表示 replace, 这是文档推荐的清除窗口内容的方式
