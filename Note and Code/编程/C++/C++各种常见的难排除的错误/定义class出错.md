错误信息
```bash
expected class-name before ‘{’ token
```
检查了好一会儿,才发现
```cpp
class SyncMsg: public BaseMSsg
{

};
```
public 后面的 BaseMSsg 搞错了,根本就没定义这个类,但编译器也没有报错说找不到这个类
