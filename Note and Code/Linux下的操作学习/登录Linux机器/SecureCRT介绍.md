### SecureCRT 简介
SecureCRT Windows 软件,用于连接Linux 服务器,这里介绍常用的配置和常见问题的解决办法(注:个人还是推荐使用 Xshell)

### 上传,下载文件路径  
使用 sz rz 命令可以在 secureCRT 上把 Windows上的文件上传到 SecureCRT远程服务器中(rz)，也可以从服务器上把文件下载下来(sz)
当使用rz时，会从一个默认路径开始查找，如果这个路径是不常用的路径，那么每次都要繁琐地跳转目录，另外执行sz命令时也会传送到一个默认目录上，我们也希望将文件传送到指定目录中，而不用再拷贝  
设置方法
options->session options ->Terminal->Xmodem/Zmodem
##### rz 使用注意事项
使用 rz 接受压缩文件如果被拒绝，一个解决的办法是加参数，加 -ybe
```bash
rz -ybe  #这样接受文件就不会被拒绝了
```

### 配置SecureCRT
SecureCRT 的相关配置可以参考以下两篇文章，可以进行美化界面等工作
配置可以在 options> session options 或 global options 处使用
https://blog.csdn.net/u014079773/article/details/69257552
##### 背景色的配置
https://blog.csdn.net/u010031673/article/details/51130889
非常好的一篇配色文章，强烈建议按照其设置来配置  
粗体颜色和细体颜色就是对应终端显示粗体字和细体字时的颜色，教程里有些颜色并没有进一步配置，如果想改我们也可以自己对着调色板改，一般来说把颜色调浅些对眼睛会舒服一些

### backspace键失灵
有时候backspace会无法起到删除的作用，而是显示 ^H ^H
解决办法：
```bash
Options–>Session Options–>Terminal–>Emulation–>Mapped Keys(中文：选项–>回话选项–>终端–>仿真–>映射键)
```
然后：
```
选中复选框  （Backspace sends delete、  Delete sends backspace）
```

### 常用技巧
文章网址:http://blog.51cto.com/jasonyong/244556
**注意脚本录制这个功能,挺实用**

1. 打开secureCRT，按alt+b，可以调出快速启动栏，我相信secureCRT的忠实用户，都会保存一堆的sessions。

2. 按ctrl，可以同时选中多个session，再点击连接，可快速连接。

3. 打开的第一个标签记为数字1，第二个记为2，依次类推，可用alt+1、alt+2……这种方式在不同标签之间切换。

4. 按钮栏，在查看->按钮栏，会在下面列出一排按钮，可以对其进行逐一的编辑，包含不同的动作，视频里演示的是一次性完成路由器的初始化配置，编辑一个按钮，选择动作是send string，输入代码是“en\r conf t\r no ip domain lo\r line con 0\r logg s\r exec-t 00\r exit\r hostname R”其中\r代表回车，都是些基本的配置。

5. 交互窗口，在查看->交互窗口，会在下面显示交互的窗口，在空白处点击右键，选择将交互发送至所有标签，这样在交互窗口输入的命令，会同时在多个标签中同时执行，有时候为了验证配置情况，需要各自show run，有了这种方法，就可以一次性搞定所有的输入了。

6. 脚本录制，脚本->开始录制脚本，在配置之前打开脚本录制，结束后停止脚本录制，保存成一个脚本文件，关闭软件后，重新打开执行之前的脚本，相当于由机器自动完成了之前的所有配置。我个人习惯于将繁琐的输入密码的过程录制一份脚本，每次启动会话执行该脚本直接进入到conf模式下，不过这样的缺点是，密码将以明文形式保存在脚本中，不够安全。

7. 键盘映射器，在选项->全局选项->默认会话设置->编辑默认的设置->找到映射键，就可以设定快捷键了，与按钮栏相类似，输入一个键，执行一个动作。

8. 锁定会话，文件->锁定会话，文件->解锁会话。可以用锁定会话，在个人离开后，避免被别人误操作。可以锁定一个或多个session。

9. sftp，在远程操作linux系统时，经常遇到需要与windows交换文件的问题，之前我的做法是使用rz命令，后来发现不是每个版本都支持rz命令，有时候搭建一个vsftpd临时解决一下，但所有的都不如sftp来的容易，secureCRT在5.5版本还不支持sftp，升级至6.2意外的发现有了sftp的支持，快捷键是alt+p，心情大爽。

10. 关闭会话，在一个标签下，按ctrl+F4，即可关闭当前标签；关闭整个程序，则是众所周知的alt+F4。

11. secureCRT 所有session文件保存的位置：options -> global options -> SSH hosts keys 最下面的目录地址里面.

12. linux 下安装lrzsz软件包(redhat自带),可以使用sz和rz命令,上传文件.

13. 显示字体: options -> global options -> default session -> edit default setting -> appearance,修改字体的大小等等.

14. 使用类似linux的虚拟终端:options -> global options -> default session -> edit default setting -> Emulation -> Terminal 选择 linux

15. 当你对一个网络进行完一个操作后，忽然发现你忘了monitor session ,或者sh run 出来将近一万行命令，屏幕往回翻，又翻不了几页，这时，你可以修改一个参数，达到效果，具体如下： Options-----Global Options---Edit default Settings进去后点Terminal----Emulation---下面的Scroll back后面的数字自己修改好了，比如我选了10000，你也可以选更大的，再点OK

16. 鼠标框选复制,右击就粘贴,putty也有这个功能.这个功能有的人刚用的时候可能很不喜欢.但是用用之后就会觉得很好用.安装之后第一次回让你选择,如果没有的话,可以在Global Options -> Terminal -> Mouse 这里改.
