### module解决的问题
加强了不同的包的管理,相同的包可以进行版本控制,这点 dep,glide 也做到了,但并不完美,因为dep,glide要把不同的包都放到当前项目的vendor目录下,这意味着不同的包不能复用,每个项目都要保存一份,此外,每个项目依然要存放在 $GOPATH/src 目录下

go mod 使得我们不再依赖 $GOPATH这个变量,项目可以放在任何文件夹下,这对开发人员是很愉快的事情,比如我有时候喜欢写一些小项目,并不想要受到条条框框的约束

### go mod 使用
go mod 要在 go1.11才开始支持,ubuntu16.04直接使用`apt-get`安装的是 go1.6,版本有些过时,为了安装go1.11需要更新 apt 的仓库网址,go在 github 的官网项目介绍了要如何操作
要介绍go mod ,首先就要介绍 go 是怎么去搜索包的,在 go.11中,这与 $GO111MODULE 这个系统变量有关
#### GO111MODULE 变量
GO111MODULE 的可以选取的值为 off, on, or auto(默认值),它们的解释如下:
* off: GOPATH mode，查找 vendor 和 GOPATH 目录
* on：module-aware mode，使用 go module，忽略 GOPATH 目录
* auto：如果当前目录不在 $GOPATH 并且 当前目录（或者父目录）下有go.mod文件，则使用 GO111MODULE，否则仍旧使用 GOPATH mode。
如果我们要在非 $GOPATH 目录下创建项目,那么可以不用去设置这个变量,只要我们对项目用`go mod init modulename`初始化作为module,则会首先在当前目录下去找包

包的管理分两种,一种是我们网上下载的开源包,还有一种是自定义包
go modules 下载的包在GOPATH/pkg/mod中,mod目录可以存放不同版本的同一种包,这样又可以让不同的项目来这里查找,而不用每个项目都拷贝一份
对于自定义的包,如果是与该项目紧密相关而非作为通用库的, 我个人习惯存放到当前项目目录下
### 实战例子
关于管理开源包,网上有很多文章,这里讲解管理自定义包的例子,因为我习惯写一些小程序,自然不希望把自己随意写的代码放到一些比较重要的目录中,而喜欢自由地在一个目录里组织自己的小程序代码
这个例子实现的功能是:我自己定义了两个包 mypack 和 mytest,就放在项目 test_mod 目录中,mypack中的pack.go 可以引用 mytest 中的 test.go 的函数
结构很简单,它做到了:
* 不需要把项目放到 $GOPATH 中
* 把自定义包放在当前项目下,并被项目引用
* 不同的自定义包还可以互相引用,就像 java 的import 那样灵活

能够做到上面几点,就可以实现灵活地开发,写一些小项目练手也很方便
下面的例子中,为了展示package,module命名的影响,我不会严格按照规范,比如module和当前目录名字就是是不同的,这样的习惯当然不好,但是这有利于我们看清go是怎么去查找package的,下面先看看目录结构:
```bash
mymod
├── go.mod
├── main.go
├── mypack
│   └── pack.go
└── mytest
    └── test.go
```
首先自己随便找个地方创建目录`mymod`,进入`mymod` 目录后,执行`go mod init test_mod`
之后会生成文件`go.mod`,里面的内容就一行
```bash
module test_mod
```
注意,`test_mod` 是之后`go import`的依据,调用 import 时会以 go.mod 为根目录开始向下找,如果找不到,才会去`$GOPATH/pkg/mod`中找

`main.go`, `mytest/test.go`, `mypack/pack.go` 的内容分别如下:
```Go
//main.go
package main

import (
    "fmt"
    _"test_mod/mypack" //去调用 mypack 包中代码的init函数
)

func main() {
    fmt.Println("hello world")
}
```
```Go
// mytest.go, Print()被 mypack/pack.go 调用,展示不同包的互相调用
package mytest

import (
    "fmt"
)

func Print() {
    fmt.Println("mytest module was used by my pack")
}
```
```Go
package mypack
import (
    "fmt"
    mytest "test_mod/mytest"
)
func init() {
    fmt.Println("mypack init")
    mytest.Print()
}
```
之后我们在 mymod 项目根目录下执行`go build`,结果生成可执行文件`test_mod`,执行这个可执行文件,显示如下
```bash
$ ./test_mod 
mypack init
mytest module was used by my pack
hello world
```
这里我们看看几个要注意的地方:
- 根目录名称是 `mymod`但是执行 go mod 时执行的是`go mod init test_mod` 说明 init 后跟的参数不一定要和根目录一致
- 在 Go 代码中, import 语句导入时,使用的是 `test_mod/xxx`,说明 import 是以 go.mod 中这个 module 名字为基准来查找的,而非目录名字