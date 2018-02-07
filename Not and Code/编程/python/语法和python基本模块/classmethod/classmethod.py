classmethod 类比与java里的静态方法，对于使用了 @classmethod 的方法，我们可以不用创建类的实例就
调用它，在类中使用了 @classmethod 的方法，默认的第一个参数是这个类的地址

例子：创建一个py文件，加入下面代码，并编译它

class A(object):
    a = 'a';
    def __init__(self):
        self.b = 'b';
    @classmethod
    def func(cls):
        print(cls)  #输出 <class '__main__.A'>
        print(cls.a) #输出 a
        #print(cls.b)

A.func()

可以看出cls即是 class A，注意cls是A的地址，因此cls.a会成功，而使用cls.b会报错，因为它不是实例，
我们要在func中创建一个A的实例，可以用 c = cls()  则c为A的一个实例
