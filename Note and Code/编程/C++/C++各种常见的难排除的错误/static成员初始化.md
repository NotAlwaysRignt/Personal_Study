有时候我们会在类中定义 static 成员,比如要加缓存
定义 static 成员变量后,一定要进行初始化
static成员变量只要初始化一次,因此最好不要放在.h文件中,因为.h文件可能会被反复包含
如果 static 成员变量不进行类外初始化,将产生非常严重的后果

请看下面的例子
```cpp
#include<iostream>
#include<vector>
using namespace std;

class A
{
    public:
        void printNumber()
        {
            cout<< m_vecNum.size() << endl;
        }
        void addNumber(int a)
        {
            m_vecNum.push_back(a);
        }
    private:
        static vector<int> m_vecNum;
};

vector<int> A::m_vecNum;

int main(int argc, char *argv[])
{
    A a;
    a.printNumber();
    a.addNumber(10);
    a.addNumber(8);
    a.printNumber();
    return 0;
}
```
编译可以通过,并且顺利运行,结果打印 0 和 2,这里我们进行了类外的初始化 
```cpp
vector<int> A::m_vecNum;
```
如果把这句话去掉会怎么样? 结果是编译通过,但是运行时会报错说 m_vecNum 未定义,这是简直是灾难