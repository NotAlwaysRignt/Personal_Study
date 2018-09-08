```bash
SRC = $(wildcard src/*.cpp):
OBJ = $(patsubst %.cpp,%.o,$(SRC))

VA:
	@echo $(SRC)
    @echo $(OBJ)

打印出来的结果是这样：
src/RedisMsg.cpp src/RedisProtocolParser.cpp src/RedisServer.cpp src/redis_test.cpp:
src/RedisMsg.o src/RedisProtocolParser.o src/RedisServer.o src/redis_test.cpp:

看起来莫名其妙，为什么 OBJ 最后一个 .o 就没被替换，百思不得其解
```
**一个很难发现的地方，输出的语句最后有一个冒号！就是那个冒号，本来是不应该有的，再看自己的makefile，在SRC变量的最后也有一个多余的冒号**
所有坑是这样来的
```bash
$(wildcard src/*.cpp)的结果：
src/RedisMsg.cpp src/RedisProtocolParser.cpp src/RedisServer.cpp src/redis_test.cpp

再加上一个冒号：
src/RedisMsg.cpp src/RedisProtocolParser.cpp src/RedisServer.cpp src/redis_test.cpp:

patsubst是在替换时会根据空格把每个单词分开来，而最后一个 redis_test.cpp: 结尾是 .cpp: 所以不会被替换为.o

```

把SRC 后面的：去掉，结果正常，简直是天坑