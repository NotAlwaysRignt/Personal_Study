import pickle
# 可以将python的对象存储在磁盘上，日后要用到这个对象时从磁盘取出，可类比java的序列化
# python对象可以存储到数据库中，但这有缺点，一旦存储到数据库中，其它程序就能访问这个对象的数据了，
# 这不符合python封装的原则，另外，有时数据库不能理解对象，会将对象转换为自己的数据类型，或用自己的关系
# 模型，这样不利于对象从数据库中转换回来
# pickle可以帮助我们将任意复杂对象转换为对象的文本或二进制文件，即将对象序列化，存入磁盘中

from dbdb.logical import LogicalBase, ValueRef

'''
 理清 BinaryNodeRef 和 BinaryNode 类的关系， BinaryNodeRef的实例指向 BinaryNode的实例
 而 BinaryNode 中又有四个数据成员，左右子树(BinaryNodeRef实例)，key(类型为str)，\
 键值 (类型为 ValueRef，它也是BinaryNodeRef的父类)
'''

# 二叉树节点：包含了 键key，键值value_ref(ValueRef的实例)，
# 左右子树引用(BinaryNodeRef实例，注意它也是ValueRef的子类)，树的高度 length
class BinaryNode(object):
    # 返回一个BinaryNode类的实例，
    @classmethod
    def from_node(cls, node, **kwargs):
        length = node.length
        if 'left_ref' in kwargs:
            length += kwargs['left_ref'].length - node.left_ref.length 
            # (node.length + kwargs['left_ref'].length - node.left_ref.length)

        if 'right_ref' in kwargs:
            length += kwargs['right_ref'].length - node.right_ref.length

        return cls(
            left_ref=kwargs.get('left_ref', node.left_ref),
            key=kwargs.get('key', node.key),
            value_ref=kwargs.get('value_ref', node.value_ref),
            right_ref=kwargs.get('right_ref', node.right_ref),
            length=length,
        )


    # left_ref、 right_ref 是 BinaryNodeRef 类型，    
    def __init__(self, left_ref, key, value_ref, right_ref, length):
        self.left_ref = left_ref
        self.key = key
        self.value_ref = value_ref
        self.right_ref = right_ref
        self.length = length

    def store_refs(self, storage):
        self.value_ref.store(storage)
        self.left_ref.store(storage)
        self.right_ref.store(storage)


# 封装的数据 引用值 self._referent, 这个引用值在磁盘中的位置 self.address
class BinaryNodeRef(ValueRef):
    def prepare_to_store(self, storage):
        if self._referent:
            self._referent.store_refs(storage)

    @property
    def length(self):
        if self._referent is None and self._address:
            raise RuntimeError('Asking for BinaryNodeRef length of unloaded node')
        if self._referent:
            return self._referent.length
        else:
            return 0

    # 这是对 ValueRef 中 referent_to_string 的重写，将对象序列化
    @staticmethod
    def referent_to_string(referent):
        return pickle.dumps({
            'left': referent.left_ref.address,
            'key': referent.key,
            'value': referent.value_ref.address,
            'right': referent.right_ref.address,
            'length': referent.length,
        })
    
    # 这是对 ValueRef 中 string_to_referent 的重写，将序列化的对象解码得到对象
    # 然后再生成一个 BinaryNode 对象
    @staticmethod
    def string_to_referent(string):
        d = pickle.loads(string)
        return BinaryNode(
            BinaryNodeRef(address=d['left']),
            d['key'],
            ValueRef(address=d['value']),
            BinaryNodeRef(address=d['right']),
            d['length'],
        )


class BinaryTree(LogicalBase):
    node_ref_class = BinaryNodeRef

    def _get(self, node, key):
        while node is not None:
            if key < node.key:
                node = self._follow(node.left_ref)
            elif node.key < key:
                node = self._follow(node.right_ref)
            else:
                return self._follow(node.value_ref)
        raise KeyError

    
    def _insert(self, node, key, value_ref):
        # node is None 出现在插入第一个树的节点时
        if node is None:
            new_node = BinaryNode(
                self.node_ref_class(), key, value_ref, self.node_ref_class(), 1)
        elif key < node.key:
            new_node = BinaryNode.from_node(
                node,
                left_ref=self._insert(
                    self._follow(node.left_ref), key, value_ref))
        elif node.key < key:
            new_node = BinaryNode.from_node(
                node,
                right_ref=self._insert(
                    self._follow(node.right_ref), key, value_ref))
        else:
            new_node = BinaryNode.from_node(node, value_ref=value_ref)
        return self.node_ref_class(referent=new_node)

    def _delete(self, node, key):
        if node is None:
            raise KeyError
        elif key < node.key:
            new_node = BinaryNode.from_node(
                node,
                left_ref=self._delete(
                    self._follow(node.left_ref), key))
        elif node.key < key:
            new_node = BinaryNode.from_node(
                node,
                right_ref=self._delete(
                    self._follow(node.right_ref), key))
        else:
            left = self._follow(node.left_ref)
            right = self._follow(node.right_ref)
            if left and right:
                replacement = self._find_max(left)
                left_ref = self._delete(
                    self._follow(node.left_ref), replacement.key)
                new_node = BinaryNode(
                    left_ref,
                    replacement.key,
                    replacement.value_ref,
                    node.right_ref,
                    left_ref.length + node.right_ref.length + 1,
                )
            elif left:
                return node.left_ref
            else:
                return node.right_ref
        return self.node_ref_class(referent=new_node)

    def _find_max(self, node):
        while True:
            next_node = self._follow(node.right_ref)
            if next_node is None:
                return node
            node = next_node
