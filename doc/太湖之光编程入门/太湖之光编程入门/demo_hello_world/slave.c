#include <stdio.h>
#include "slave.h"

__thread_local int my_id;

void func(void* paras)
{
    my_id = athread_get_id(-1);
    printf("hello world from athread id %d\n", my_id);
}
