先直接上脚本
```bash
#!/bin/sh

#测试机登录
MachineIp=XXX.XXX.XXX.XXX
User=username
Password=123456786

#登录后进入的目录
EnterPath=/usr/local/mydir

main(){
    expect -c "
        spawn ssh $User@$MachineIp  -t \"cd $EnterPath; 
                                    source ~/mydir/.bashrc;
                                    bash --login\"
        expect {
            \"*password:*\" {send \"$Password\r\";}
            }
        interact
        expect eof"
}

main
```

* 虽然没有安装send只安装了expect,可以使用expect内置的 send ,采用 -c 的方式,send 输入完密码后加一个\r,效果相当于回车
* 必须有interact 这个命令,表示交互模式,用户会停留在远程服务器上面,否则会错误地在登录后退出
* ssh -t 表示登录成功后执行的一系列命令,上面的例子中,登录后还会进入到指定目录,并且执行指定的.bashrc文件,注意最后的 bash --login不可以漏掉


写好ssh文件后,假设命名为 login.sh,直接用source命令(. 命令)即可
```bash
. login.sh
```
