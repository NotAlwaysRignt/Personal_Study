#include "Std.h"
#include <iostream>
#include "unistd.h"
#include <stdlib.h>

/*
 * 此处并没有包含 hook.h
 * 因此理论上调用的sleep 应该是 unistd.h 的,malloc 和 free 应该是 unistd.h 的
 * 如果最终标准库中的函数没有被调用,则说明被hook了
*/

//如果没有被hook,则不会打印任何东西
void Std::Sleep()
{
  sleep(5); // sleep for 5 seconds
}

//如果没有被hook,则不会打印任何东西
void Std::MallocAndFree()
{
  int *p = (int*)malloc(5*sizeof(int));
  if(p!=NULL)
    free(p);
}
