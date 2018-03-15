1. 借助 apt-get 命令安装 nodejs 和 npm  
```bash
sudo apt-get install nodejs-legacy
sudo apt-get install npm
```

这样安装的 nodejs 和 npm 往往不是最新的,我们需要升级它们   

升级npm为最新版本
sudo npm install npm@latest -g

node有一个模块叫n，是专门用来管理node.js的版本的。使用npm（NPM是随同nodejs一起安装的包管理工具）安装n模块  
```bash
sudo npm install -g n  
```
然后，升级node.js到最新稳定版
```bash
sudo n stable  
```

