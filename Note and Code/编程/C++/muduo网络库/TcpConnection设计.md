### TcpConnection需要解决的问题
#### 串话现象
POSIX 要求每次新建文件描述符时选取当前最小可用的整数. 那么可能会出现以下问题,假设服务器正在处理一个连接 A,fd = 8,这时客户端把连接关闭了. 同时新的客户端请求到来,创建了连接B,这时连接 B 的 fd 恰好就用了 刚刚回收的 fd=8
这时 fd=8 还是可以用的,如果连接 A 继续发送 response, 则发送到了连接B的客户端上去了,这就是串话
串话的情景告诉我们:仅用 fd 来标记一个 TCP 连接是不准确的.
TcpConnection 的设计目的之一就是解决串话问题,用 对象而非 fd 来标记一个连接

#### 生命周期
TCP 连接的声明周期不总是服务器控制,因为客户端可以主动关闭连接.这使得生命周期具有不确定性.对于声明周期不确定的对象,只能分配到堆上
声明周期的不确定使得我们难以把握 delete 对象的时机,即便客户断开了连接,也可能还有其它地方引用这个对象,贸然delete 会导致指针悬空.因此,引用计数是解决这种情况的有效手段. TcpConnection 就是用 shared_ptr 来管理的
