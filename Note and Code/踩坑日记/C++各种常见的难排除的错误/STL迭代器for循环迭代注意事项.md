### 使用 erase 时应改变迭代策略

STL中的容器按存储方式分为两类，一类是按以数组形式存储的容器（如：vector 、deque)；另一类是以不连续的节点形式存储的容器（如：list、set、map）。在使用erase方法来删除元素时，需要注意一些问题

在使用 list、set 或 map遍历删除某些元素时可以这样使用：  
方法1:
```cpp
std::list< int> List;
std::list< int>::iterator itList;
for( itList = List.begin(); itList != List.end(); )
{
      if( WillDelete( *itList) )
      {
            itList = List.erase( itList);
       }
       else
            itList++;
}
```

或者这样使用:  
方法2:
```cpp
std::list< int> List;
std::list< int>::iterator itList;
for( itList = List.begin(); itList != List.end(); )
{
      if( WillDelete( *itList) )
      {
          List.erase( itList++);
      }
      else
          itList++;
}
```
核心问题在于 iter ++ 不可以写在for的()里,否则很可能导致for 循环变成了死循环


**vector 和 deque 不可以这样使用上面的方法2,只能采用如下形式**
```cpp
1 for (iter = cont.begin(); iter != cont.end();)
2 {
3    (*it)->doSomething();
4    if (shouldDelete(*iter))
5       iter = cont.erase(iter); 
6    else
7       ++iter;
8 }
```
##### erase造成的问题详解
1. 对于关联容器(如map, set, multimap,multiset)，删除当前的iterator，仅仅会使当前的iterator失效，只要在erase时，递增当前iterator即可。这是因为map之类的容器，使用了红黑树来实现，插入、删除一个结点不会对其他结点造成影响。
2. 对于序列式容器(如vector,deque)，删除当前的iterator会使后面所有元素的iterator都失效。这是因为vetor,deque使用了连续分配的内存，删除一个元素导致后面所有的元素会向前移动一个位置。还好erase方法可以返回下一个有效的iterator。


##### func(x++)的效果是怎么样的
接下来分析吧自增放在for的 {} 里和放在 ()里语法上有何区别

自增放在for循环的内部,即{}内部,和放在for的括号()内有何区别,我们知道如果我们把自增放到括号内部
```cpp
for(...; ...; x++)
{
	//TODO
}
```

x\+\+ 相当于在 {}里的所有内容执行完后才执行,我们知道在map,List等结构中,调用完erase 后迭代器就失效了,这个时候再去自增也增不到正确地位置了,如果是调用函数时执行
func(x\+\+),实际情况如何:可以看下面的例子
```cpp
int *p;
void test(int i)
{
    cout<<i<<endl;
    cout<<*p<<endl;
}
int main(int argc, char *argv[])
{
    int a = 0;
    p =&a;
    test(a++);
    return 0;
}
```
打印的结果是
```bash
0
1
```
显然 \+\+ 是在进入函数前就进行了,那么如果我们使用
erase(iter\+\+)
iter 会先自增并保留到 iter 中,然后原来的 iter 作为参数传入 erase 中,erase后失效,也不会影响后面的迭代了