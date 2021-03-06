## 第1章:关于 go 语言的介绍
#### 开发速度
* 比静态语言C++,Go 开发快,编译速度很快,不用像 Java,C++ 那样遍历所有依赖链中的依赖库,而只关注直接被引用的库
* 比起动态语言 python, Go 有类型检查,能在编译器捕获类型错误

####　并发
* go语言不用像其它语言那样使用多线程来利用多个核
* 并发的难点在于确保并发线程不会意外地修改用户的数据,在没有同步系统保护下不同线程修改同一个数据(全局变量,共享内存数据)会发生灾难,使用其它语言我们要为此制定复杂的同步规则
* Go 使用 goroutine 来支持并发,goroutine 占用内存远少于线程.一个线程可以使用多个 goroutine,常常可以启动成千上万个 goroutine.
* channel　是一种数据结构,作用是保证同一时刻只有一个 goroutine 修改数据,而不需要锁

#### 类型系统
GO 语言使用组合,抛弃继承,这样用户不必再费心思去考虑设计抽象类

#### 内存管理
将内存管理交给编译器去做,这会有一些额外的开销

## 第2章 快速开始一个 Go 程序
#### main包
##### 编译成可执行文件
要将 go 编译成可执行程序的充分条件:
1. 必须实现名称为 `main` 的函数
2. 这个 `main` 函数必须放在名为 `main` 的 `package` 里,即 main 函数所在文件开头也必须有`package main`
不满足以上条件就不会生成可执行文件

##### import
* import 使得用户可以使用 package 里的标识符,Go语言不允许 import package 后却不用里面的标识符(不必所有都用,但至少要使用一个),比如`import hello`,如果不用hello 里面的任何标识符,那么会报错并且无法通过编译.如果想要 import 却不使用其标识符,要在 package 前面加一个下划\_,如例子中`import`里的`_` "github.com/goinaction/code/chapter2/sample/matchers"`,如果加了下划线,则只导入 init 函数,其它所有标识符也不会导入,这种情况下如果要使用其标识符也使用不了
使用下划线来导入 package 目的通常是我们希望调用这个 package 下的 init 方法,但我们不需要用到这个 package 下的符号
* 名为`init`的函数会在`main`函数执行前调用
* 当导入package时,package中的init()函数会被调用
* 同一个文件夹下的所有代码文件,必须使用同一个package 名,惯例上 package 名和文件夹名相同

#### search包
##### 环境变量
编译器会到 GOROOT 和 GOPATH 两个环境变量去查找包, GO 相关的环境变量可通过命令 `go env` 来查看.
GOROOT 是 go 的安装路径,比如我的系统中go 安装到了 `/usr/lib/go-1.6/` 下面有 bin,lib 等目录

##### package 级别变量
变量如果写在函数外,则它是一个包级变量,一个包里的标识符(函数,变量等)有公开和不公开之分, 当代码导入包时,这段代码就可以使用所有公开的标识符
**标识符的公开与否是通过大小写来判别的**,如果标识符首字母是大写字母,则是公开标识符,否则(小写字母开头或写划线)标识符不公开,代码导入package后不能使用

##### 变量初始值
Go中,所有变量都被初始化为其零值,数值类型为0,布尔类型为 false,指针为 nil
`:=`是简化变量声明运算法,用于声明变量并作初始化,编译器会根据函数返回值类型来确定每个变量的类型.它只是一种简化记法,和使用 var 声明没有区别

##### 迭代
`for range`可以迭代数组,字符串,切片,映射和通道.使用`for range`迭代时会返回两个值,第一个值是当前迭代的索引位置,第二个是元素值的副本,用 `for range`会有拷贝的开销,也无法修改原数据,如果要避免拷贝开销,应该直接用索引访问

##### 下划线
下划线在import中的作用上面已经介绍,不再赘述,对于函数返回值,如果有多个返回值,且有的返回值我们不想使用它,可以用下划线`_`接受这个变量值,就像占位符一样

##### 闭包作用域
Go 的作用域和 C/C++的 `{}`作用域约定是一样的

##### defer
`defer`关键字后跟的语句会在函数返回时才执行.常用于 open close 这样的成对操作,使用 defer 缩短了 open,defer 的距离,提高了代码可读性
同一个函数里有多个defer,按defer出现的顺序,后面出现的 defer 会在函数返回时先执行

## 第4章:数组,切片和映射
#### 数组
数组,在内存是连续分配的
数组是定长的
与C++ 不同的是,取数组变量传递时并不是传递首指针,在函数中传递数组是有值拷贝开销的,因此在传递数组时应该取数组的指针来作为函数参数
#### 切片
切片与数组不同的是,切片是可变长的
切片有指针,长度,容量,三个概念,初始化时应该指定长度和容量,如果只指定长度,那么长度和容量相等
切片与数组都可以通过字面量初始化,区别在于切片的`[]`内不写任何东西,而数组必须在`[]`内填长度或`...`
```go
slice = make([]int,5)
newSlice = slice[1:3] //长度为2,容量为4 !
```
#### 映射
迭代映射时顺序是不一样的,indie映射的实现使用了散列表,键和值都放在同一个数组空间里

## 第5章 Go语言的类型系统
#### 初始化
Go 语言默认使用 0 值初始化. 习惯上,如果使用关键字 var ,则表示这个变量进行零值初始化
#### type
type 关键字可以声明一个新的类型,新声明的类型与原类型是独立的,假设有两个新声明的类型 `type A int` 和 `type B int`,A 类型的变量也不可以赋值给 B 类型,编译器不会做隐式转换
#### 方法
在关键字 func 和函数名间可以添加这个函数的接受者,从而为类型绑定方法
* 值接收者使用值的副本调用方法,指针接收者使用实际值调用方法
* 引用类型(切片,映射,通道,接口,函数类型)使用值接收者即可共享引用指向的变量,而不需要用指针类型
* C语言中函数返回值不可以是局部变量的地址,因为局部变量在栈上,结束函数后被回收了,但GO 语言可以,因为GO语言会做逃逸分析,如果发现变量作用域超出了这个函数,就会把它分配到堆中
#### 接口
指针接收者实现接口的方法时,只能使用指针给接口赋值
值接收者给接口实现的方法时,给接口赋值既可以用值也可以用指针
原因: 编译器并不是总能解析值的地址,比如一个函数接收int类型的参数,如果传递`myfun(52)`,那么编译器是没办法解析这个int类型的地址的
#### 嵌入类型
使用内部类型,一个重要的特性是内部类型的提升,即内部类型实现了接口的方法,那么外部方法也可以对接口赋值,如果外部类型没有另外实现这个接口,那么则可以调用到内部类型的实现

## 第6章 并发
并发是指同时管理很多事情,但在某一个时间点可以只做一件事,并行是同时做很多事情,某一个时间点做多件事
Go语言默认会为每个逻辑处理器绑定一个逻辑调度器.gorountine可以并行,但是必须要多于一个逻辑处理器,同时机器必须是有多个物理处理器的
使用gorountine,关键字`go`+ 匿名函数 ,或者 `go` + 函数名
#### 通道
使用go提供的通道可以解决竞争状态(race condition)遇到的问题
通道的创建使用make函数和chan关键字,`变量 := make(chan 类型)`或 `变量 := make(chan 类型,缓冲区大小)`
通道分为有缓冲的通道和无缓冲区的通道
通道的发送和赋值使用操作符`<-`,举例
```go
buffered := make(chan string,10)
buffered <- "Hello world"  //发送

value := <- buffered  //赋值
close(buffered) //通道变量可以用close将其关闭
value ,ok := <- buffered  //通道变量也可以用两个变量来接收,当chan关闭时,若 !ok 为 true,则可知通道已关闭
```
* 无缓冲通道:没有保存值功能的通道,要求发送方或接收方都准备好,如果接收方没有去接收,发送方会阻塞,如果发送方没发送,接收方会接收
* 有缓冲通道:可以理解为一个有同步功能的先进先出队列,使用make创建通道时,第二个参数决定了缓冲区大小(队列大小),通道没有值时,接收方会阻塞,通道已满时,发送方会阻塞.通道有值,则接收方可以取得返回,通道未满.则发送方发送后直接返回

当通道被 close 后,依旧可以从中取出缓(先到网上看看相关教程)存的值,如果没有缓存值,在读取通道时也会立即返回一个通道类型的零值,我们可以在读取通道时用两个变量接收它,如`task,ok := <- tasks`如果通道已经关闭,则 `!ok` 为true

## 第7章 并发模式
主要三个实际工程中用的包,runner,pool,worker,这三个包是本书自己实现的而非系统库,主要为我们如何在并发模式下组织代码带来启发
### runner
runner 包用来监视程序运行时间,它可以设置程序超时时间,当超时会返回,可以捕获系统的 interrupt 方法
适用场景: cron 作业,定时任务

## 第8章 标准库
标准库源码会被安装到 `$GOROOT/src/pkg`中,标准库的源码是经过预编译的,我们在 `$GOROOT/pkg` 目录下找到各种 .a 文件

### log 包
log 包是多 goruntine 安全的,因此多个 goroutine 可以同时调用来自同一个日志记录器的函数而不会有彼此间的写冲突
#### iota 关键字
iota 是 go 语言的常量计数器,只能在常量const中使用,相当于一种语法糖,常用在枚举中

iota 有两个特性,
1. 为每个常量复制相同的表达式,知道声明区结束,或者遇到一个新的赋值语句
2. 每次处理为常量后,都会自增 1

简单的用法
```go
const (
    a = 0
    b = 1
    c = 2
)

//用iota的写法 
const (
    a = iota  //0
    b         //1
    c         //2
)
```

看 标准库中 log/log.go 中的使用
```go
const (
	Ldate  = 1 << iota
	Ltime
	Lmicroseconds
	Llongfile
	Lshortfile
	LUTC
	LstdFlags     = Ldate | Ltime 
)
//相当于
const (
	Ldate  = 1 << iota //1<<0 == 00000001 == 1
	Ltime              //1<<1 == 00000010 == 2
	Lmicroseconds      //1<<2 == 00000100 == 4
	Llongfile          // 8
	Lshortfile         // 16
	LUTC               // 32
	LstdFlags     = Ldate | Ltime  //到这里 iota 终止了,直接计算 LstdFlags = 3 
)
```

### Json
json包应该用标签,如果不存在标签,编码和解码程序就会视图以大小写无关的方式,直接使用字段的名字进行匹配,如果无法匹配,对应结构类型里的字段就为零值

## 第9章 测试和性能
### 测试
go 语言支持的测试包括 单元测试和基准测试,使用 `go test` 命令可以用来执行写好的测试代码,但要遵守一些规则
* 测试文件必须以`_test.go`为结尾
* 测试函数必须以`Test`为开头,比请函数的参数必须接收一个指向`testing.T`类型的指针,并且不返回任何值

`t.Fatal` 方法会终止当前测试函数的执行,如果有其它 Test 函数,则会执行其它 Test 函数

### 示例
为代码生成文档可以使用 `go doc` 命令,这里不再多做讲解,要为代码生成文档可以将文档写在代码里,但要遵循一定风格,如函数名要以 Example 开头,Example后必须是基于已经存在的公开的函数名等

### 基准测试
基准测试函数必须以 Benchmark 开头,接收一个指向 testing.B 类型的指针作为唯一参数,函数不返回任何值
基准测试为了准确测试性能,会在一段时间(默认1秒,可通过`-benchtime`设置)反复运行这个函数
使用`ResetTimer`方法会重置计时器,跳过初始化代码的执行时间
