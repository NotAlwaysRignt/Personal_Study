流程图写在flow代码块中,观察下面一个例子  
\`\`\`flow  
st=>start: Start  
op=>operation: Your Operation  
cond=>condition: Yes or No?  
e=>end  
st->op->cond  
cond(yes)->e  
cond(no)->op  
\`\`\`  
**注意以下三点**  
1. 上面代码中每一行的末尾都不可以有空格,即要换行直接按回车,不可以先加空格再回车  
2. "=\>"前后都不可以有空格,比如 e<=end,"e","end"和"<=" 之间都不能有空格
3. 冒号后面必须有一个空格,比如"st=\>start: Start", "start:"与"Start"之间有一个空格  
 

最终结果如下:  
```flow
st=>start: Start
op=>operation: Your Operation
cond=>condition: Yes or No?
e=>end
st->op->cond
cond(yes)->e
cond(no)->op
```  
流程图很简单,先定义变量,然后再用"-\>"连接各个框,定义变量格式为:  
**tag=>type: content:>url**  
tag:标签名称, type:标签类型,它决定了流程框图中框的形状  
content:流程框里的内容(**再次强调type后的冒号与文本之间一定要有个空格**)  

以上面的" st=>start: Start " 为例, st即元素名称,start是流程框的类型(框图中表现为椭圆形),Start是其内容  

type是这个元素的类型，有6中类型，分别为：

    start # 开始
    end # 结束
    operation # 操作
    subroutine # 子程序
    condition # 条件
    inputoutput # 输入或产出