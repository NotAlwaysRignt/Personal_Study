### 使用 apt 安装 wireshark
Ubuntu 中安装 wireshark 很简单:  
sudo apt install wireshark  

中间会弹出一个选项,大意是 普通用户不能够打开网卡设备进行抓包,但wireshark 不建议用户通过sudo在root权限下运行
Should non-superusers be able to capture packets?   
选择 YES 即可

下载完毕后
我们还不能在终端输入 wireshark, 因为即使启动了也会提示没办法正常使用,因为有的功能没有权限   

### 把当前用户添加到 wireshark 的用户组  
我们可以在终端输入 sudo wireshark 以正常使用 wireshark, wireshark 不建议我们在超级用户下运行它,我还要去修改 /usr/share/wireshark/init.lua 文件,具体操作见下方[常见问题](#problem)

我们先查看是否已经创建了 wireshark 这个用户组,我们可以用 
```bash
sudo vim /etc/group
```  
去查看是否有 wireshark 这个组创建(在 vim 中查找 wireshark 字符串以快速找到其位置)
当然更简单的方式是:
```bash
cat /etc/group |grep wireshark  
wireshark:x:131:
```
发现已有这个用户组  

这时我们在终端输入 groups 命令,可以发现默认情况下我们当前的用户并不和 wireshark 同组(我的用户名为thinker)
```bash
thinker@thinker:~$ groups
thinker adm cdrom sudo dip plugdev lpadmin sambashare
```

接下来我们要做的是把自己（当前登录用户）加入到wireshark组中：
```bash
sudo usermod -a -G wireshark $USER
```  
这时候用groups 还不能看见 wireshark ,我们需要注销用户,重新登录,这样上面的命令才开始生效
重新登录后直接运行wireshark即可，不需要加sudo。  



### <span id="problem">安装与使用问题</span>
1. 如果打开发现提示没有权限的错误，请确认当前登录用户是否在wireshark组中或拥有root权限。查看当前登录用户所在组：
$ groups
USER adm cdrom sudo dip plugdev lpadmin sambashare wireshark

2. 如果在安装预配置wireshark-common时选择了no怎么办？  
重新配置这个包：

3. $ sudo dpkg-reconfigure wireshark-common
如果不想打开这项功能，又不想以root权限运行wireshark怎么办？  
用sudo dumpcap进行抓包，抓好之后再在wireshark里面打开刚才抓的包进行分析。

4. 使用 sudo 启动时,发现提示 
```bash
Lua: Error during loading:
[string "/usr/share/wireshark/init.lua"]:46: dofile has been disabled due to running Wireshark as superuser.  
```  
解决方法
修改/usr/share/wireshark/init.lua
```bash
# vi /usr/share/wireshark/init.lua
```
将倒数第二行：
```lua
dofile(DATA_DIR.."console.lua")
```
改为
```lua
--dofile(DATA_DIR.."console.lua")
```  
即把它注释掉
