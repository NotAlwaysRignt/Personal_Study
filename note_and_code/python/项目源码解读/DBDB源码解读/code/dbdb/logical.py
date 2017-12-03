'''
    ValueRef类将由 binay_tree.py 中的 BinaryNodeRef 类继承，BinaryNodeRef 类中未定义
__init__函数，所以构造函数参数会传入到这个类中
    封装的数据 引用值 self._referent, 这个引用值在磁盘中的位置 self.address
'''

class ValueRef(object):
    def prepare_to_store(self, storage):
        pass
    #转换为字节流
    @staticmethod
    def referent_to_string(referent):
        return referent.encode('utf-8')
    #从字节流转换回来
    @staticmethod
    def string_to_referent(string):
        return string.decode('utf-8')
        
    # self._referent 可能存储数据库的键值value，也可能存储引用 BinaryNodeRef的实例
    def __init__(self, referent=None, address=0):
        self._referent = referent 
        self._address = address  # 这个address拿到的是数据在磁盘文件的地址位置

    @property
    def address(self):
        return self._address

    # 最终将从磁盘中获取字典{'left': referent.left_ref.address,'key':...}
    def get(self, storage):
        # 在创建数据库后且第一次插入数值之前的时候， self._referent为 None，且此时 self._address
        # 存储的是超级块的根地址 即 0，注意此时 if self._referent 相当于if False 
        if self._referent is None and self._address:
            self._referent = self.string_to_referent(storage.read(self._address))
        return self._referent
    
    # 将数据(格式为长度+数据)写入磁盘文件中，并保留这个引用的位置到 self._address
    def store(self, storage):
        if self._referent is not None and not self._address:
            self.prepare_to_store(storage)
            self._address = storage.write(self.referent_to_string(self._referent))

'''
    LogicalBase类是个抽象类(因为有node_ref_class = None)，它无法被直接实例化
    在interface.py中，我们可以看见 self._tree = BinaryTree(self._storage)
但 BinaryTree 并没有定义 __init__方法，因此，self._storage 这个 Storage 类对象会传到子类
LogicalBase 中，self._storage 存储的是Storage对象(封装了文件描述符，可以进行文件的操作，并
在增删操作中提供数据格式的转换功能)
    BinaryTree中保存着数据库的文件描述符( self._storage = storage),并始终存储着
根节点(封装在 BinaryNodeRef中 )，这个根节点也会存储在磁盘文件的开始位置
'''
class LogicalBase(object):
    node_ref_class = None  # BinaryTree 中，有 node_ref_class = BinaryNodeRef 进行了覆盖
    value_ref_class = ValueRef

    def __init__(self, storage):
        self._storage = storage
        self._refresh_tree_ref() 
        
    def commit(self):
        self._tree_ref.store(self._storage)
        self._storage.commit_root_address(self._tree_ref.address)
 
    def _refresh_tree_ref(self):
        self._tree_ref = self.node_ref_class( 
            address=self._storage.get_root_address())
            # 生成一个 BinaryNodeRef 类的实例 self._tree_ref，BinaryNodeRef 是ValueRef的子类
            # address=self._storage.get_root_address()传到 ValueRef的__init__函数中
            
    def get(self, key):
        if not self._storage.locked:
            self._refresh_tree_ref()
        return self._get(self._follow(self._tree_ref), key)

    def set(self, key, value):
        if self._storage.lock():
            self._refresh_tree_ref()
        self._tree_ref = self._insert(
            self._follow(self._tree_ref), key, self.value_ref_class(value))
    # self._insert 在 BinaryTree 中实现
    
    
    def pop(self, key):
        if self._storage.lock():
            self._refresh_tree_ref()
        self._tree_ref = self._delete(
            self._follow(self._tree_ref), key)
    
    # 参数 ref 是 ValueRef 的实例，
    # 得到 self._referent，如果 self._referent不存在，而 self._address 存在
    # 根据ref中的地址self._address 从磁盘中读取 ref引用 的值 self._referent
    def _follow(self, ref):
        return ref.get(self._storage)

    def __len__(self):
        if not self._storage.locked:
            self._refresh_tree_ref()
        root = self._follow(self._tree_ref)
        if root:
            return root.length
        else:
            return 0
