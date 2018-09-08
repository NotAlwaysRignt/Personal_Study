项目中写单元测试其实是有必要的,后台开发中,如果我们等整个工程编译好再放到测试机上跑起来,然后还要构造请求来测,若测试不通过,改完代码,编译,将编译后的文件放到测试机,配置启动,这个流程非常复杂,效率低下.
因此我试图找到一个比较便捷的方法,最好就像 java 那样,一键编译运行测试某个模块

编写单元测试的目标:
* 所有输出可执行文件和测试代码都放在 test 目录下,而不应该污染工程
* 在代码分离的情况下,测试代码应该和项目代码能够一块编译
* 支持一键编译,运行

最终方案如下:
项目根目录中创建一个test目录,test目录下创建一个bin用于存放输出,目录中另写一个Makefile放置于test 目录中,注意,所有的相对路径,包括 include makefile.comm 中涉及的相对路径都是相对于执行make命令时的路径,想要一键运行,必须有VimAsynRun 配合

为了能够让测试文件和项目文件一块编译,最终决定,先编译项目,生成的中间.o文件放在 obj 目录下,之后test目录下的测试代码和 .o 文件一块编译,这样的好处是不用在测试时又为原来的项目生成另外的.o文件,同时编译整个项目的时候先把语法错误给调了

下面makefile的缺点是需要为每个测试文件写一个目标以及该目标的依赖,我们不可以指定目标的依赖是 obj 下所有的 .o 文件,因为有些 .o 文件一块编译后会报错(是用于生成动态库的,无法生成可执行文件)
我们只要让makefile中的目标遵循一定的命名规则(与文件名相同),就可以在 vim 中配置每次都 make 当前文件名的目标,这样就能一键运行了

项目中引用了googletest,可以专门为googletest写 -I 和 -L 选项,放在一个makefile.comm里
```makefile
include ./makefile.comm
include ~/tools/commlib/googletest/makefile.comm
 
#如果项目根目录的makefile 添加了INC 和LIB 路径,test目录下的makefile也要保持一致
INC += -I../include
LIB += 

#把组内微线程库包含进来,可以独立执行微线程库的API
MT_LIB:= -L/usr/local/commlib/platformlib/spp_plugin/test_tool/lib -lmt

CC = gcc 
CXX = g++ 
OBJ_DIR = ./obj/
SRC_DIR = ./src/
 
#输出到bin目录下,这与vim里AsyncRun的配置要保持一致,便于一键执行
OUTPUT_DIR = ./test/bin/
 
.PHONY: test test_init
 
# TEST_FLAGS was first defined in makefile.comm of googletest
TEST_FLAGS += -DTEST_DEBUG=0 -DZZCFG_DEBUG=0  -Wall -pipe -g -fPIC -O0 -std=c++0x 
 


init:
    mkdir -p ./test/bin
 
# 通用的模板,只需要改目标名称和依赖的.o文件即可,目标名称应和测试文件名(去掉后缀)相同,这是为了便于使用 vim 的 AsyncRun快捷键键执行
test_boss:$(OBJ_DIR)boss_parser.o
    $(CXX)  -o  $(OUTPUT_DIR)$@.out  $(TEST_FLAGS) ./test/$@.cpp  $^ $(LIB)  $(INC) 
 
test_string:
    $(CXX)  -o  $(OUTPUT_DIR)$@.out  $(TEST_FLAGS) ./test/$@.cpp  $^  $(LIB)  $(INC) 
```

vim中相关配置如下:
```bash
" F5运行.out文件
nnoremap <silent> <F5> :AsyncRun -raw -cwd=$(VIM_FILEDIR) "$(VIM_FILEDIR)/bin/$(VIM_FILENOEXT).out" <cr>

"定义 F9 为创建bin目录并编译当前光标所在文件,将可执行文件(.out后缀)输出到bin目录下
nnoremap <silent> <F9> :w<cr>:AsyncRun mkdir -p bin && g++ -Wall -g "$(VIM_FILEPATH)" -Iinclude -o "$(VIM_FILEDIR)/bin/$(VIM_FILENOEXT).out" <cr>
```

我们可以把googletest 和 googlemock 按照如下结构组织到某一个目录下
```bash
.
├── include
│   ├── gmock
│   │   ├── gmock.h
│   │   ├── ...
│   │   └── internal
│   │       ├── ...
│   └── gtest
│       ├── gtest-death-test.h
│       ├── gtest.h
│       ├── ...
│       └── internal
│           ├── custom
│           │   ├── gtest.h
│           │   ├── ...
├── lib
│   ├── libgmock.a
│   └── libgtest.a
└── makefile.comm
```
makefile.comm可以用如下形式编写
```makefile
CURRENT_DIR = /data/home/chaoranxu/tools/commlib/googletest/
INC+= -I$(CURRENT_DIR)include -I$(CURRENT_DIR)include/gmock -I$(CURRENT_DIR)include/gtest
LIB+= -L$(CURRENT_DIR)lib -lgtest -lgmock
```