# This started as a very thin wrapper around a file object, with intent to
# provide an object address on write() and a superblock. But as I was writing
# it, I realised that the user really wouldn't want to deal with the lengths of
# the writen chunks (and Pickle won't do it for you), so this module would have
# to abstract the file object into it's own degenerate key/value store.
# (Degenerate because you can't pick the keys, and it never releases storage,
# even when it becomes unreachable!)

import os
import struct

import portalocker


# 这个类封装了文件描述符，使文件可以安全地进行读写，还负责在读写时完成格式的转换

class Storage(object):
    #超级块是取自文件系统的术语，在这里由它保存整个数据库文件的一些基本信息，一般超级块的长度会设置为
    # 1024 B 的整数倍，我们指定它为 4096 字节
    SUPERBLOCK_SIZE = 4096
    INTEGER_FORMAT = "!Q"
    INTEGER_LENGTH = 8
    
    # 参数f为文件描述符
    def __init__(self, f):
        self._f = f
        self.locked = False
        self._ensure_superblock()  # 开辟超级块(在文件开头创建4096字节空间，数据全为0)

    # 这个函数在 Storage 类被创建时调用，用于在文件开头开辟一个4096字节的空间作为超级块，
    # 这里 4096字节空间并没有全部被用上，只有开头部分存取了一个整数，是二叉树的根地址
    def _ensure_superblock(self):
        self.lock()
        self._seek_end()
        end_address = self._f.tell()
        if end_address < self.SUPERBLOCK_SIZE:
            self._f.write(b'\x00' * (self.SUPERBLOCK_SIZE - end_address))
        self.unlock()
    
    # 如果没锁，就把文件锁上
    def lock(self):
        if not self.locked:
            portalocker.lock(self._f, portalocker.LOCK_EX)
            self.locked = True
            return True
        else:
            return False
    
    # 文件解锁
    def unlock(self):
        if self.locked:
            self._f.flush()
            portalocker.unlock(self._f)
            self.locked = False
    
    # os.SEEK_END = 2  seek(offset,MODE)，seek(0,2)意味移动到相对文件结尾前的第0个字符
    # 即该函数的作用为移动到文件尾部
    def _seek_end(self):
        self._f.seek(0, os.SEEK_END)
    
    # f.seek()的第二个参数默认为0,即意味从起点开始，第一个参数0意味着移动到离起点处第0个位置
    # 所以该函数作用是移动到文件开头
    def _seek_superblock(self):
        self._f.seek(0)
    
    # 如其函数名字，将字节流转换为整数数组(是一个set)，并返回第一个元素
    def _bytes_to_integer(self, integer_bytes):
        return struct.unpack(self.INTEGER_FORMAT, integer_bytes)[0]

    def _integer_to_bytes(self, integer):
        return struct.pack(self.INTEGER_FORMAT, integer)
    
    # f.read(n), 从文件指针开始的位置读入n个字节，因为在struct.pack中一个整数会被转换成8个
    # 字节的数据, 所以这个函数会从文件中读入一个整数
    def _read_integer(self):
        return self._bytes_to_integer(self._f.read(self.INTEGER_LENGTH))

    # 将传入的整数写入文件中
    def _write_integer(self, integer):
        self.lock()
        self._f.write(self._integer_to_bytes(integer))

    # 在文件尾先写入数据的长度(字节流形式)，再写入数据，然后返回当前文件的位置
    # 注意这个data已经是被转换成字节流的
    def write(self, data):
        self.lock()
        self._seek_end()
        object_address = self._f.tell()
        self._write_integer(len(data)) # 这个字节流的长度
        self._f.write(data)
        return object_address
    
    # 先读处文件的长度，然后根据长度读出数据,这个数据是之前序列化的字典{...}
    def read(self, address):
        self._f.seek(address)
        length = self._read_integer()
        data = self._f.read(length)
        return data
    
    # 在写入文件时，并不是直接操作磁盘，只有等缓冲区满了才会输出到磁盘，这样做是为了提高效率
    # 这个函数先将缓冲区全部数据输出到文件，然后来到文件的开头，写入根地址，再输出到缓冲区
    def commit_root_address(self, root_address):
        self.lock()
        self._f.flush()
        self._seek_superblock() # 来到文件头
        self._write_integer(root_address) # 将根节点地址值写入
        self._f.flush()
        self.unlock()
    
    #查找到开头，然后读取开头的第一个整数，即树根的地址root_address
    def get_root_address(self):
        self._seek_superblock()
        root_address = self._read_integer()
        return root_address
    
    def close(self):
        self.unlock()
        self._f.close()

    @property
    def closed(self):
        return self._f.closed
