from dbdb.binary_tree import BinaryTree
from dbdb.physical import Storage


# DBDB只是定义了逻辑上的接口，如查增删改，开启和关闭数据库，提交文件到磁盘

class DBDB(object):

    def __init__(self, f):
        self._storage = Storage(f)
        self._tree = BinaryTree(self._storage)

    def _assert_not_closed(self):
        if self._storage.closed:
            raise ValueError('Database closed.')

    def close(self):
        self._storage.close()

    def commit(self):
        self._assert_not_closed()
        self._tree.commit()

    def __getitem__(self, key):
        self._assert_not_closed()
        return self._tree.get(key)

    def __setitem__(self, key, value):
        self._assert_not_closed()
        return self._tree.set(key, value)

    def __delitem__(self, key):
        self._assert_not_closed()
        return self._tree.pop(key)

    def __contains__(self, key):
        try:
            self[key]
        except KeyError:
            return False
        else:
            return True

    def __len__(self):
        return len(self._tree)
        
'''
数据库初始化干两件事
1. 生成 Storage 类的实例 (封装了数据库的文件描述符)，并且在第一次创建文件的同时就在文件开头创建了
一个超级块(4096字节的空间，数值全部为0)
2. 生成 BinaryTree 类的实例， BinaryTree 在创建的时候初始化了两个成员变量，一个是self._storage
存储了 Storage 类的实例，另一个是self._tree_ref， 这时一个 BinaryNodeRef 类的实例，
self._tree_ref 里面有两个重要的成员变量 self._referent (此时还是None)，self._address (文件根节点)
在本例中 self._tree_ref 的 self._address 值是超级块的地址，即 0


这里介绍两个过程 __setitem__ ，和 __getitem__ 
先介绍 __setitem__ ：
假设一开始 BinaryTree 中无任何节点， 可知 key 和 value 都是 str 类型
则 __setitem__(self, key, value) 中调用了  self._tree.set(key, value)
set 函数 在 BinaryTree 的子类 LogicalBase 中

def set(self, key, value):
    if self._storage.lock():
        self._refresh_tree_ref()
    self._tree_ref = self._insert(
        self._follow(self._tree_ref), key, self.value_ref_class(value))
此时并未锁住，self._storage.lock() 会加一把写锁，然后返回True，接着调用 self._refresh_tree_ref()
在BinaryTree中给 self._tree_ref 赋值 (BinaryNodeRef 类的实例)，它封装了根节点的地址，但其内部
self._referent 为 None

我们先看 self._insert， 它在 BinaryTree 中被实现

self.value_ref_class = ValueRef， value 的值会传递到内部的 self._referent 中

先看 self._follow(self._tree_ref)
根据上面的介绍self._tree_ref 是一个 BinaryNodeRef 类的实例，里面存有成员变量 _tree_ref (此时为None)
和 _address (超级块的地址 0 )

所以，上面的self._tree_ref 是一个对根地址(此时值为0)的封装 BinaryNodeRef 的对象实例
再看self._follow, 它得到 BinaryNodeRef 中的 self._referent，在存入第一个数据的时候 self._tree_ref
是None(因为我们在调用self.node_ref_class时只传入了address)，且self._address为0,因此self._fellow
返回的值也为None,综上，我们调用了 self._insert(None,key，封装了root地址的value_ref)


def _insert(self, node, key, value_ref):
    if node is None:
        new_node = BinaryNode(
            self.node_ref_class(), key, value_ref, self.node_ref_class(), 1)
        ......#
        return self.node_ref_class(referent=new_node)
        
node_ref_class = BinaryNodeRef

因此 new_node 是一个 BinaryNode 实例 ,它内部有左右子树引用 BinaryNodeRef （内部self._referent均
为None，有一个key值， value_ref 是 ValueRef类的实例， 树高length = None)
这里我们注意到，键值value 用 ValueRef封装，指向左右子树的引用 BinaryNodeRef(ValueRef的子类)封装
最后 将这个 BinaryNode用 BinaryNodeRef 封装 实例化后返回

综上，数据库插入第一个值后，BinaryTree 中 self._tree_ref  为一个 BinaryNodeRef 实例，其中
self._address = 0, self._referent是一个 BinaryNode类的实例
这个 BinaryNode内部数据: 左右子树 BinaryNodeRef(内部的 self._referent均为None，address为0)，
ValueRef (内部的self._referent存储了value，self._address = 0)，key值

接下来在tool.py文件中，我们可以看到在执行一次插入操作后(db[key] = value)，会进行数据的提交
db.commit()， db是DBDB类的实例，commit定义如下，我们来看看它干了什么：

def commit(self):
    self._assert_not_closed()
    self._tree.commit()
沿着 self._tree 找到 LogicalBase，再找到 
def commit(self):
    self._tree_ref.store(self._storage) # 根节点才会调用的方法
    self._storage.commit_root_address(self._tree_ref.address)

_tree_ref即 ValueRef，找到store方法
def store(self, storage):
    if self._referent is not None and not self._address:
        self.prepare_to_store(storage)
        self._address = storage.write(self.referent_to_string(self._referent)) 
注意 if self._referent is not None and not self._address 意味着这必须是根节点的值


在 BinaryNodeRef 中找到        
def prepare_to_store(self, storage):
    if self._referent:
        self._referent.store_refs(storage) 

找到 BinaryNode
def store_refs(self, storage):
    self.value_ref.store(storage)
    self.left_ref.store(storage)
    self.right_ref.store(storage)
#这里相当与一个递归，一直进行下去直到树的最底层

回到 ValueRef 中的self._address = storage.write(self.referent_to_string(self._referent))
referent_to_string 在 BinaryNodeRef 中被重写， self._referent 是根节点 BinaryNode
write函数会先跳转到文件的结尾，获取地址存储到 address中，这就更新了跟节点的地址，
然后写入这个数据的长度(字节流的长度)，再将数据写入，比如下面
这里会将
{
    'left': referent.left_ref.address,
    'key': referent.key,
    'value': referent.value_ref.address,
    'right': referent.right_ref.address,
    'length': referent.length,
} 序列化并存入文件中，注意，这里的键值要么是地址，要么是字符串，并没有对象实例，这样可以减少存储的空间

self._storage.commit_root_address(self._tree_ref.address)
上面已经更新了根节点指向数据的地址，这里再把它更新到磁盘文件的开头

总结：
db.commit()

此时 self._tree_ref.address 存储的是跟节点的地址
'''        
        

