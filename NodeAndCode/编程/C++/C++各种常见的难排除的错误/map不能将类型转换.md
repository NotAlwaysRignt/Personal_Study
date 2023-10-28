错误输出如下
```cpp
错误：将‘const std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >’作为‘_Tp& std::map<_Key, _Tp, _Compare, _Alloc>::operator[](const _Key&) [with _Key = std::basic_string<char, std::char_traits<char>, std::allocator<char> >, _Tp = std::basic_string<char, std::char_traits<char>, std::allocator<char> >, _Compare = std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, _Alloc = std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > >]’的‘this’实参时丢弃了类型限定
```

C++ 的stl 报错是非常难调试的
注意开头第一个字const
在 map 中,const map 的条件是很苛刻的

```cpp
map<int,int> mapTmp;
cout<<map[1]<<endl;
```
mapTmp 并没有初始化过,但调用 map[1]却可以,因为key 不存在时,会自动扩容
所以对于const map, 是不能使用[] 这种用法的