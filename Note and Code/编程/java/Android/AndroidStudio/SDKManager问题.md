Android Studio 官方版本不一定自带 SDK,要自己去下载 SDK Manager,启动时告诉你 missing SDK ,你就要自己去下载,然后在 Settings 里 Edit SDK 的路径,指向解压缩后的SDK 文件夹(这个文件夹下一定有一个 tools文件夹)
解压后在 tools 下有一个可执行的 android 文件, ./命令执行它,注意**一定要用sudo权限去执行它**  
如果你发现找不到很多 System Image 文件,本来应该找到并能够下载的,那么就很可能因为你没有使用sudo命令


要使用Android Studio开发,有几个工具是必须安装的,如 Platforms ,具体安装哪些见网上


即便能连到外网很多软件也不一定能下,加个国内镜像吧,打开菜单栏 Tools>option   
HTTP Proxy Server:mirrors.neusoft.edu.cn  
HTTP Proxy Port: 80  
把下面 force https:.../source 勾选上

这样就可以享受快速下载了,注意有时会下载失败,这时把VPN关了再试一下!
