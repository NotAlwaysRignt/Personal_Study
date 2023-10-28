主要参考阮一峰的博客:
http://www.ruanyifeng.com/blog/2015/02/make.html

make这个词，英语的意思是"制作"。make命令直接用了这个意思，就是要做出某个文件
make 命令更多应用在代码编译的场合,用来规定各个文件编译的先后顺序,编译的规则,当然我们也可以把shell命令写入其中,只要符合 makefile 文件的语法,它就会去执行

### 为什么要 make 
对于单一文件的编译,我们可能直接调用gcc就可以了,如果有多个文件相互联系,那我们可能需要在终端多次敲打 gcc 命令,还要注意编译的顺序,以及编译的规则(比如哪些文件先生成.o文件,哪些.o文件要被清理等)  
对于一个大型项目,如果我们在开发过程中要反复做这些事情,简直就是灾难,因此,我们可以把所有的命令都写进 makefile 文件中,然后使用 make 命令使其自动帮我们执行这些命令

### make命令
如果我们直接调用我们调用 make 命令,那么它会在当前目录下寻找名为"makefile"的文件并执行它,如果没有找到就寻找名为"Makefile"的命令并去执行它.  
当然我们可以通过 "make -f 文件名" 的形式指定具体文件名

### makefile文件格式
Makefile文件由一系列规则（rules）构成。每条规则的形式如下。
```bash
    <target> : <prerequisites> 
    [tab]  <commands>
```
**注意command前必须要有一个Tab键,我们很可能会在自己的编辑器中,将tab替换为4个空格或多个空格,make只认Tab键,如果没有用tab键,则会报"missing separator"的错误,用多个空格make并不会当作tab键来处理,所以如果不替换tab键,那就记得自己编辑器的设置**
 
假设 makefile 中有如下内容:
```bash
a.txt: b.txt c.txt
    cat b.txt c.txt > a.txt
```
我们调用 make a.txt ,则它会找到 "a.txt:" 并执行此处命令  
第一步，确认 b.txt 和 c.txt 必须已经存在，第二步使用 cat 命令 将这个两个文件合并，输出为新文件。

再回到这个文件格式  
```bash
    <target> : <prerequisites> 
    [tab]  <commands>
```
上面第一行冒号前面的部分，叫做"目标"（target），冒号后面的部分叫做"前置条件"（prerequisites）；第二行必须由一个tab键起首，后面跟着"命令"（commands）。

"目标"是必需的，不可省略；"前置条件"和"命令"都是可选的，但是两者之中必须至少存在一个。

每条规则就明确两件事：构建目标的前置条件是什么，以及如何构建。下面就详细讲解，每条规则的这三个组成部分。
#### 目标（target）

一个目标（target）就构成一条规则。目标通常是文件名，指明Make命令所要构建的对象，比如上文的 a.txt 。目标可以是一个文件名，也可以是多个文件名，之间用空格分隔。

除了文件名，目标还可以是某个操作的名字，这称为"伪目标"（phony target）。

```bash
clean:
      rm *.o
```
上面代码的目标是clean，它不是文件名，而是一个操作的名字，属于"伪目标 "，作用是删除对象文件。

```bash
$ make  clean
```
但是，如果当前目录中，正好有一个文件叫做clean，那么这个命令不会执行。因为Make发现clean文件已经存在，就认为没有必要重新构建了，就不会执行指定的rm命令。

为了避免这种情况，可以明确声明clean是"伪目标"，写法如下。

```bash
.PHONY: clean
clean:
        rm *.o temp
```
声明clean是"伪目标"之后，make就不会去检查是否存在一个叫做clean的文件，而是每次运行都执行对应的命令。像.PHONY这样的内置目标名还有不少，可以查看手册。

如果Make命令运行时没有指定目标，默认会执行Makefile文件的第一个目标。

```bash
$ make
```
上面代码执行Makefile文件的第一个目标。
#### 前置条件（prerequisites）

前置条件通常是一组文件名，之间用空格分隔。它指定了"目标"是否重新构建的判断标准：只要有一个前置文件不存在，或者有过更新（前置文件的last-modification时间戳比目标的时间戳新），"目标"就需要重新构建。

```bash
result.txt: source.txt
    cp source.txt result.txt
```
上面代码中，构建 result.txt 的前置条件是 source.txt 。它会检查当前目录 source.txt 文件是否存在，如果存在,那么"cp source.txt result.txt"就会被运行，如果不存在,它会检查当前 makefile文件中是否有 "source.txt:" 这个target,如果有,先执行这个target下的内容,然后再执行"cp source.txt result.txt"  
如果既不存在 source.txt 这个文件,也不存在 source.txt 这个 target,则会报错.  
我们可以写一个target来尝试一下，来生成 source.txt 。

```bash
source.txt:
    echo "this is the source" > source.txt
```
上面代码中，source.txt后面没有前置条件，就意味着它跟其他文件都无关，只要 source.txt这个文件还不存在，每次调用make source.txt，它都会生成。

```bash
$ make result.txt
$ make result.txt
```
上面命令连续执行两次make result.txt。第一次执行会先新建 source.txt，然后再新建 result.txt。第二次执行，Make发现 source.txt 没有变动（时间戳晚于 result.txt），就不会执行任何操作，result.txt 也不会重新生成。
"make: 'result.txt' is up to date."

#### 命令（commands）

命令（commands）表示如何更新目标文件，由一行或多行的Shell命令组成。它是构建"目标"的具体指令，它的运行结果通常就是生成目标文件。

每行命令之前必须有一个tab键。如果想用其他键，可以用内置变量.RECIPEPREFIX声明。
**再次强调,我们很可能会在自己的编辑器中,将tab替换为4个空格或多个空格,make只认Tab键,用多个空格make并不会当作tab键来处理,所以如果不替换tab键,那就记得自己编辑器的设置**
```bash
.RECIPEPREFIX = >
all:
> echo Hello, world
```
上面代码用.RECIPEPREFIX指定，大于号（>）替代tab键。所以，每一行命令的起首变成了大于号，而不是tab键。

需要注意的是，每行命令在一个单独的shell中执行。这些Shell之间没有继承关系。

```bash
var-lost:
    export foo=bar
    echo "foo=[$$foo]"
```
上面代码执行后（make var-lost），取不到foo的值。因为两行命令在两个不同的进程执行。一个解决办法是将两行命令写在一行，中间用分号分隔。

```bash
var-kept:
    export foo=bar; echo "foo=[$$foo]"
```
另一个解决办法是在换行符前加反斜杠转义。

```bash
var-kept:
    export foo=bar; \
    echo "foo=[$$foo]"
```
最后一个方法是加上.ONESHELL:命令。

```bash
.ONESHELL:
var-kept:
    export foo=bar; 
    echo "foo=[$$foo]"
```
