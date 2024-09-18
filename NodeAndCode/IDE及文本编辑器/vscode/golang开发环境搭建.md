[TOC]
# 背景
介绍如何在vscode搭建自己的远程开发环境，通过SSH直连开发机，不需要将文件拷贝到本地
**chatgpt是强大的顾问，它对vscode非常了解，有更多需求不妨尝试问问它**

# 远程连接 remote-ssh
安装插件 remote-ssh，配置好远程IP地址
这里最好是一次配置，后续免密登录，要在本地和远程都装好git
本地`C:\Users\myuser\.ssh`创建密钥，把公钥上传到云开发机上的`~/.ssh/authorized_keys`目录下(没有这个文件就新建一个)

# 插件和配置
## go 插件
vscode go 基本覆盖了所有能力，这里先用 remote-ssh 登录到远程，然后可以在远程安装插件
相当于开发机上会安装一个 vscode server，go插件也会安装到开发机上，同时开发机依赖的 golps 这些也会在连接时远程安装到开发机上，如果发现装好后补全什么没成功，重启重登录触发试试，或者重启开机，这里之前自己开发机的磁盘满了（某个bug），重启开发机就恢复了

## vim 插件
安装vim插件，vscode对vim的集成非常友好，很多快捷键都是一致的，直接安装即可
### easymotion
安装vim插件，这里注意的是vim插件已经继承了`easymotion`，默认是关掉的的，需要在 vim 相关配置中将其打开，我们不再需要安装其他的motion插件了

### NERDTree
vscode 有NerdTree 的插件，安装后修改快捷键`Ctrl-N`为自己习惯的按键即可

### 快捷键
vim的快捷键，包括其他插件的快捷键，会与系统快捷键冲突，这里如果发现不符合预期，需要在快捷键设置里找到对应快捷键，检查冲突，对不需要的快捷键解绑
需要关注的快捷键有，vim的`Ctrl-W`会与系统冲突，优先改为用vim的
#### 文件
vscode `Ctrl-P`是系统默认的文件搜索，已经足够我们使用
注意，本人习惯在vim的`normal`模式下，使用`\f`作为打开文件搜索的快捷键，这里修改快捷键后，需要在快捷键设置中，修改`When`的条件，点击右键再点击`Change When Expression`，输入`editorTextFocus && vim.active && vim.mode == 'Normal'`，否则在insert模式下`\`会输入不了
#### 跳转
本人习惯使用 `ctrl-]`跳转定义（安装vim后会支持），并用`ctrl-o`返回跳转，这里需要在快捷键中修改 `ctrl-o` 对应指令为`Go Back`

### 搜索
vscode集成了各种模糊搜索，我们不需要像vim一样去装`leaderf`了

#### word 搜索
但是我们想纯靠键盘来使用其Search功能，则需要定制化快捷键，打开快捷键配置`Keyboard Shortcuts`,tab右侧有一个编辑`keybindings.json`选项，我们可以修改json来设置自定义快捷键
下面是我自定义的快捷键选项，它在vim normal 模式下，会快速选中当前光标对应的单词，然后选取输出到 vscode 的search 框中
补充说明，我们在search框中，通过`Ctrl-Downright`可以跳到下面的结果，这样就不需要鼠标了
```json
    {
        "key": "f3",
        "command": "vim.remap",
        "args": {
            "before": ["<F3>"],
            "after": ["v", "i", "w"],
            "commands": [
                {
                    "command": "workbench.action.findInFiles",
                    "args": {
                        "query": "${selectedText}",
                        "triggerSearch": true,
                        "isRegex": false,
                        "matchWholeWord": true,
                        "isCaseSensitive": false
                    }
                },
                "widgetNavigation.focusNext"
            ]
        },
        "when": "editorTextFocus && vim.active && vim.mode == 'Normal'"
    },
```
### 编译
我们使用vim的`:sh`就可以快速拉起终端，然后自己进行go build 编译
通过`Ctrl-D`进行终端销毁
默认`Ctrl-Shift-M`可以通过Problem面板查看代码检测的各种问题，连续按下可以进行面板的拉起与关闭

### 配置示例
下面是需要修改的相关配置，让它和我的习惯结合
```json
    "editor.fontSize": 16,
    "debug.console.fontSize": 16,
    "markdown.preview.fontSize": 16,
    "files.autoGuessEncoding": true,
    "workbench.editorAssociations": {
    },
    "remote.SSH.remotePlatform": {
        "Devcloud": "linux"
    },
    "vim.easymotion": true,
    "vim.insertModeKeyBindings": [
        {
            "before": ["j", "j"],
            "after": ["<Esc>"]
        },
        {
            "before": ["/", "/"],
            "after": ["/", "/", " "]
        }
    ],
    "vim.normalModeKeyBindingsNonRecursive": [
        {
            "before": ["/", "/"],
            "after": ["/", "/", " "]
        },
        {
            "before": ["j"],
            "after": ["g", "j"]
        },
        {
            "before": ["k"],
            "after": ["g", "k"]
        },
        {
            "before": ["$"],
            "after": ["g", "$"]
        },
        {
            "before": ["J"],
            "after": ["g", "j"]
        },
        {
            "before": ["K"],
            "after": ["g", "k"]
        },
        {
            "before": ["<Space>"],
            "after": ["<Leader>", "<Leader>", "s"]
        }
    ],
    "vim.handleKeys": {
        "<C-n>": false,
        "<C-d>": true,
        "<C-s>": false,
        "<C-z>": false
    },
    "vim.commandLineModeKeyBindings": [

    ],
    "editor.rulers": [
        120
    ],

```