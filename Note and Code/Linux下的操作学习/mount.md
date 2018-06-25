### mount命令  
linux 要访问一个文件系统时,首先就要对它进行挂载,linux中最顶级的文件目录就是根目录 /  

不同的文件系统有不同的文件格式,访问规则自然不同,linux 几乎支持所有文件系统, 常用的文件系统格式为 ext2 和 ext3,但如果想要访问其它文件系统资源的话,比如 USB ,CD,就需要使用 mount命令  
很多 linux 操作系统,比如 Ubuntu,在我们插入硬件设备时就已经自动帮助我们完成了文件的挂载(一般会将windows的C盘,插入的U盘等挂载到/media目录下),所以我们不需要手动去mount
  
#### mount 的用法
标准的mount使用如下
```bash
mount -t type device dir
```  
-t <文件系统类型> 指定设备的文件系统类型  

####　实例

Windows 的C分区挂到Liunx的/mnt/winc目录下:   
Windows 的文件系统采用的是 vfat 格式
```bash
mkdir ~/desktop/winc # 在用户桌面创建 winc 目录 
sudo mount -t vfat /dev/hda1 ~/desktop/winc  # 设C分区分配于设备 hda1 
```
这样即可在~/desktop/winc 中查看C分区里的文件

卸载设备
```bash
sudo umount ~/desktop/winc
```
winc将变回一个空的普通目录
