参考资料:
[x86-64 下函数调用及栈帧原理](https://blog.csdn.net/lqt641/article/details/73002566)
[libco协程库上下文切换原理详解](https://blog.csdn.net/lqt641/article/details/73287231)

### x86体系函数调用栈帧原理
首先讲讲x86架构中 %rbp 和 %rsp 的作用:
* %rbp 是栈帧指针，用于标识当前栈帧的起始位置
* %rsp 是堆栈指针寄存器，通常会指向栈顶位置，堆栈的 pop 和push 操作就是通过改变 %rsp 的值即移动堆栈指针的位置来实现的。

%rbp,%rsp存放的值是栈的地址,相当于指针,而不是栈的数据,注意区别.

%rsp(栈顶) 和 %rbp (栈底)构成了一段栈,该栈存放了一个函数的栈上分配的数据,随着数据不断地被压入栈中(比如函数参数,以及临时栈上分配变量), %rsp 里的数值不断变化,**注意 x86 体系栈空间采用的是向下增长的方式,地址值由高向低,栈顶(%rbp)不动,栈底向(%rsp)下增长**

`x86-64`架构中函数调用过程:
1. 父函数将调用参数从后向前压栈(是否从后向前实际中依赖于编译器,在gcc中是从后向前)
2. 将函数返回地址入栈
3. 跳转到子函数开始执行(在汇编中相当于 jump 到指定的符号标记处往下执行代码)
4. 子函数将父函数栈帧起始地址(%rpb 中的值)压栈
5. 将 %rbp 的值设置为当前 %rsp 的值，即将 %rbp 指向子函数栈帧的起始地址,由第4步可知此时 %rbp 指向的数据是父函数的起始地址(上一栈帧的%rbp),这样就可以从子函数返回父函数。

上述步骤中,返回地址入栈和跳转到子函数处执行由 call 一条指令完成，在call 指令执行完成时，已经进入了子程序中
而第4,5步操作(上一栈帧 %rbp 压栈的操作及修改%rbp的值)需要在汇编中由子函数完成

函数返回时,只要`mov esp,ebp`,栈顶指向栈底,接着`pop ebp`,%ebp 的值恢复到父函数的 %ebp 的值,这样父函数的栈就恢复了,以上两个操作在 x86 中等效于`leave`指令,`ret`指令使得代码返回到上次调用`call`的地方

示例:
```bash
...   # 参数压栈
call fun  # 将返回地址压栈，并跳转到子函数 FUNC 处执行
...  # 函数调用的返回位置

fun:  # 子函数入口
pushq %rbp  # 旧的栈低指针值存入栈中
movq  %rsp, %rbp  # 让 %rbp 指向新栈帧的起始位置.栈顶改变,相当于构建了新的栈
subq  $N, %rsp  # 在新栈帧中预留一些空位，供子程序使用，用 (%rsp+K) 或 (%rbp-K) 的形式引用空位
```

问题:压入栈中的函数参数在子函数栈低 %rbp 的上面,即不在 %rbp 和 %rsp 指针指向的范围内,那么子函数运行过程中如何取得这些值.
答: 子函数运算过程中取的数据并不一定要位于%rbp 和 %rsp 指针指向的范围内,%rbp 和 %rsp 存放的本质上是地址值,是个参考位置,如果在栈底之外,可以使用负值,比如 -16(%rbp),这样就可以取到函数参数了,至于负值是多少,编译器会计算出来生成对应的汇编代码

有了上面的基础,接下来看看libco是如何实现上下文切换的
### libco协程上下文切换
首先看看libco协程切换的api使用方法:
```cpp
void A() {
   cout << 1 << " ";
   cout << 2 << " ";
   co_yield_ct();  // 切出到主协程
   cout << 3 << " ";
}

void B() {
   cout << “x” << " ";
   co_yield_ct();  // 切出到主协程
   cout << “y” << " ";
   cout << “z” << " ";
}

int main(void) {
  ...  // 主协程
  co_resume(A);  // 启动协程 A,这是示意简写,实际不能直接传函数指针
  co_resume(B);  // 启动协程 B
  co_resume(A);  // 从协程 A 切出处继续执行
  co_resume(B);  // 从协程 B 切出处继续执行
}

// 打印结果: 1 2 x 3 y z
```
可见核心的 API 是`void co_yield_ct()`和`void co_resume( stCoRoutine_t *co )`,分别用于切出协程和恢复协程
`co_yield_ct`和`co_resume`最终都会调用`void co_swap(stCoRoutine_t* curr, stCoRoutine_t* pending_co)`,而`co_swap`中会调用`coctx_swap`函数
其声明`void coctx_swap( coctx_t *,coctx_t* ) asm("coctx_swap");`
第一个参数是要切出的协程,第二个线程是切出后要进入的协程
C++无法从语言层面上支持协程,需要汇编代码的支持,此处是协程实现的核心
这个函数是用汇编实现的,代码存放于`coctx_swap.S`文件中,对应了`i386`版本和`x86-64`版本的实现,这里只列出`x86-64`对应的实现
```c
coctx_swap:
#if defined(__i386__)
// ......
#endif

#elif defined(__x86_64__)
	leaq 8(%rsp),%rax
	leaq 112(%rdi),%rsp
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx

	pushq -8(%rax) //ret func addr

	pushq %rsi
	pushq %rdi
	pushq %rbp
	pushq %r8
	pushq %r9
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	
	movq %rsi, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r9
	popq %r8
	popq %rbp
	popq %rdi
	popq %rsi
	popq %rax //ret func addr
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rsp
	pushq %rax
	
	xorl %eax, %eax
	ret
#endif
```
首先看前两句
```c
	leaq 8(%rsp),%rax
	leaq 112(%rdi),%rsp
```
`leaq 8(%rsp),%rax`表示将 %rsp 的值 + 8 后存入 %rax 中,这是一个地址值, 这一地址是父函数`co_swap`栈帧中返回地址+8的位置,+8则意味着将返回地址除外

问题:为什么这里 %rsp 就是对应父函数返回地址,不可能是临时栈变量的地址吗?
答:我们这里的子函数是`coctx_swap`,是调用`co_swap`时进入的,而由上面代码可知,此时只是调用了两个`leaq`语句,并没有任何入栈出栈操作,因此 %rsp 指向的就是返回父函数的地址(即子函数返回后父函数继续执行的位置)

%rdi, %rsi, %rdx, %rcx,%r8, %r9 六个寄存器用于存储函数调用时的6个参数(如果有6个或6个以上参数的话)
此处 %rdi 存储的是父函数`co_swap`栈帧中调用`coctx_swap`时的第一个参数,其类型为`coctx_t *`,保存的是当前协程的协程,定义如下
```cpp
struct coctx_t
{
#if defined(__i386__)
	void *regs[ 8 ];
#else
	void *regs[ 14 ];
#endif
	size_t ss_size;
	char *ss_sp;
	
};
```
这里讨论的是`x86-64`的实现,取`regs[ 14 ]`,已知`x86-64`中指针类型占 8 字节,14*8(regs)=112,因此 112(%rdi) 的值为第一个协程参数 `coctx_t` 中 `void *regs[14]` 数组后的地址

这里详细解释一下: `coctx_t* curr`对应的地址是`struct coctx_t`这个结构体的起始地址,如果要取`regs[0]`,则可以用 (void *)curr做强制转换,这样就取得了reg[0],由此可得,`curr+112`取到了`ss_size`的地址(要以 8 字节取出来)

通过上面讨论得知,我们在 %rax 中存储了(父函数返回地址+8)值,这个地址值指向的是第一个参数`coctx_t*`的值,这个指针指向当前切换协程. %rsp 则存储了第一个参数`coctx_t*`的`ss_size`的地址

接下来看
```c
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx

	pushq -8(%rax) //ret func addr

	pushq %rsi
	pushq %rdi
	pushq %rbp
	pushq %r8
	pushq %r9
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
```
上面一共有14个`pushq`语句,对应数组`void *regs[14]`的位置,`pushq %rax`表示把 %rax 存入`regs[13]`,后面以此类推.

问:由上得 %rsp 此时存储的是 regs 第14个元素后的位置(%rdi + 112),为什么 %rax 却被存入`reg[13]`,而不是`size_t ss_size;`?
答:从地址与数据对应的角度去理解比较困难,不妨从结果返推,%rdi 存储的是当前协程`coctx_t*`的值,如果我们要获取`void* regs[14]`数组的所有内容,当 `%rsp = %rdi`,即指向了该地址时,如果我们要取出所有的内容,则我们要执行14次pop,执行完后 %rsp 的值为 `%rsp+14*8 = %rsp+112`,由此就容易理解了,要从regs[13]开始赋值,则应该从`%rdi+112`开始执行push

赋值完毕后,我们可以得到 `regs[0]`~`regs[13]`的数值情况
```c
// 64 bit
//low | regs[0]: r15 |
//    | regs[1]: r14 |
//    | regs[2]: r13 |
//    | regs[3]: r12 |
//    | regs[4]: r9  |
//    | regs[5]: r8  | 
//    | regs[6]: rbp |
//    | regs[7]: rdi |
//    | regs[8]: rsi |
//    | regs[9]: ret |  //ret func addr, 对应 rax
//    | regs[10]: rdx |
//    | regs[11]: rcx | 
//    | regs[12]: rbx |
//hig | regs[13]: rsp |

```
通过上述操作,相当于把当前协程中所有寄存器的数值存储到栈空间中

切换协程后,要恢复原来协程中所有寄存器的状态,即将存储在栈中的寄存器数据取出来放入寄存器中,其思路与入栈相反
```c
	movq %rsi, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r9
	popq %r8
	popq %rbp
	popq %rdi
	popq %rsi
	popq %rax //ret func addr
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rsp
```
`movq %rsi, %rsp`一句,`%rsi`存储的是`void coctx_swap( coctx_t *,coctx_t* )`第二个参数的值,存储了目标协程的信息,%rsp 切换后,调用了14次`popq`,将原来协程的寄存器状态恢复.

只是恢复了寄存器的内容并不能完成协程的切换,我们还要将代码运行的地址跳转到目标协程,看最后三句
```c
	pushq %rax
	
	xorl %eax, %eax
	ret
```
%rax 的值由`popq %rax`得到,对应倒数第5次`popq`,我们找到对应的第五次`pushq`操作为`pushq -8(%rax)`,执行push 时 %rax 存放的值为(父函数返回地址+8),因此`-8(%rax)`即父函数的返回地址值,对应的是调用完`coctx_swap`后函数的继续运行处的地址.综上,此时 %rax 存放的是目标协程的代码运行地址,也是恢复的运行点.
 xorl 把 %rax 低32位清0以实现地址对齐,最后ret 语句用来弹出栈的内容，并跳转到弹出的内容表示的地址处,而弹出的内容正好是上面 pushq %rax 时压入的 %rax 的值，即之前保存的此协程的返回地址。即最后这三条语句实现了转移到新协程返回地址处执行，从而完成了两个协程的切换。
 
这里对 ret 的行为做详细解释,进入子函数时,%rsp 一开始使指向父函数返回地址值,接着在子函数里我们可能会创建一些临时变量,有入栈操作,使 %rsp 变化,当子函数运行完毕后,创建的临时变量会逐个出栈,最终rsp又指向父函数地址值,执行 ret 命令时,相当于先将代码运行地址(IP寄存器的值)改为 %rsp 的值,然后这个父函数地址值出栈(%rsp+4),这样就实现了函数的跳转了
对应上面的行为,先push了 %rax,里面存放了目标协程的地址,在执行ret,就可以将这个%rax出栈并跳转到 %rax 对应的代码运行处

#### 总结
协程的切换包括两部分: 寄存器数据的恢复,代码运行地址的切换. 切出协程的数据全部存入栈中(`struct coctx_t`的`regs`),并从栈中取出目标协程的数据存入寄存器数据,另外栈中也存放了目标协程代码的运行地址,根据地址值即可跳转到地址处继续运行.
完成这两步,就实现了目标协程的切换了
