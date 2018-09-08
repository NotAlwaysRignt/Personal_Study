VScode 的好处在于,我们选择打开一个文件夹而非工作区,VScode也可以帮我们解析,进而方便我们写代码.它不用像Visual stdio 一样要创建一个工程
但是直接打开一个文件夹来看这个文件夹内的源代码,VSCode 是不会知道文件夹间的包含关系的,比如头文件的位置,因此需要我们去手动设置

要设置就需要设置文件,这个设置工程属性文件的名称叫做c_cpp_properties.json, 我们打开文件夹后(将这个打开文件夹时选定的目录称为工作区目录 workspaceFolder),工作区目录下会创建两个目录,.vs目录和.vscode目录,我们可以在 .vscode 目录下手动创建名为c_cpp_properties.json 的文件

另一种方法是,如果找不到头文件,则 include 语句就会有下划线标记,并告诉我们找不到头文件,先点击那句include 语句,,会看见 include 旁边有个小灯泡的图标,点击后会出现 (Edit "includePath" setting)  等选项,点击(Edit "includePath" setting),就会自动在.vscode目录下创建 c_cpp_properties.json 文件并让我们进行设置


具体的配置方法在下面看下面的网站,这里只看出一份简单的示例,通常情况下,只要指向头文件的路径,那么就足够阅读代码了
https://github.com/Microsoft/vscode-cpptools/blob/master/Documentation/LanguageServer/c_cpp_properties.json.md

##### 示例
${workspaceFolder} 指的是打开文件夹时所选择的文件夹,使用这个变量时,子目录可以用单斜杠/ 来表示,如下面的${workspaceFolder}/include/**

**配置示例,# 对于windows上的目录,要用双斜杠 \ \表示目录关系**
```json
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**",
                "${workspaceFolder}/include/**",
                "E:\\CompanySrcCode\\spp_include\\**"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "windowsSdkVersion": "",
            "intelliSenseMode": "clang-x64"
        }
    ],
    "version": 4
```