#include <iostream>
#include "hook.h"
#include <dlfcn.h>

typedef void (*hook_free_pfn)(void* ptr);
static hook_free_pfn g_hook_free = (hook_free_pfn)dlsym(RTLD_NEXT, "free");

unsigned int sleep(unsigned int seconds)
{
  std::cout << "This is Hook API sleep" << std::endl;
  return 0;
}

void free(void *ptr)
{
  std::cout << "This is Hook API free" << std::endl;
  g_hook_free(ptr);
}

int enable_hook()
{
  return 0;
}
