#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include "hook.h"
#include "Std.h"

using namespace std;
//该程序展示了 Std.cpp 中的 sleep 和 free 函数是如何被 hook 的

int main(int argc, char *argv[])
{
  enable_hook();
  Std s;
  s.Sleep();
  s.MallocAndFree();
  return 0;
}
