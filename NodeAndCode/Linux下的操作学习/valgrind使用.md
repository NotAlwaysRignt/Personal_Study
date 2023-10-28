假设有个可执行文件为 test_exe.out
一般只要带上以下三个选项即可
```bash
valgrind  --tool=memcheck --leak-check=full --show-reachable=yes ./test_exe.out
```

*  -leak-check=no|summary|full 要求对leak给出详细信息? [summary]
*  -show-reachable=no|yes show reachable blocks in leak check? [no]

另外当valgrind检查出来的错误较多时，建议使用
valgrind --tool=memcheck --leak-check=full --log-file=memcheck.log ./your_process
保存到日志文件里。

**注意,使用 valgrind 调内存泄漏时,一定要把可执行文件放在所有 valgrind 选项的最后面,不然会出现有泄漏但不报错的现象**
