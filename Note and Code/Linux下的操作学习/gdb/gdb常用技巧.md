### 启动gdb  
```bash
gdb ./main # main是可执行文件,表示用gdb启动它
```  
进入gdb命令行,这个时候程序还不会运行  

### 查看代码  
l 可以列出代码及其行号,我们才知道在哪里打断点  
```bash
l 30  #表示从30行开始显示代码
```   
### 自动补全  
gdb中支持对当前环境中已经存在的变量或者gdb命令进行自动补全,按下Tab即可补全,如果可补全的选项有多个,多按几次Tab可进行列出  

### 设置断点
```bash
breakpoint  代码行数 #某一行打断点  
info breakpoints #查看已经打了的断点及其相关信息
clear 断点编号 #清除已经打下的断点,断点编号通过 info breakpoints查得  
condition 断点编号 条件 #设置条件断点
```
条件断点关键字condition可简写为cond,常用于循环语句中,此时要注意断点的位置
```cpp
for(int i=0; i<5;i++){
	cout<<i<<endl;
}
//要使 i == 3 时停下,断点应该设置在 cout<<i<<endl;这一行而不是 for那行
//假设断点编号为2 则命令为 cond 2 i==3
```

### 监控变量  
print(简写 p) + 变量名, 打印当前环境下的变量  
display(简写disp)+变量名 同样是显示变量,但是使用disp 显示的
#### 查看数组  
查看数组有特殊的语法,比如
```cpp
int n =100
int *p = new int[n];
```
要在gdb中查看p的内容,可以这样打印  
```bash
print *p@50 #查看前数组p的前50个元素  
print *p@n #打印p的前n个元素(int n=100)
#n是程序中的变量,只要在当前环境中存在,就可以在gdb中使用
```  
