#include <athread.h>
#include <stdio.h>
#include "memory.h"

double sarr[64000];

extern SLAVE_FUN(func)();

void c_doge_(double* arr)
{
    printf("enter host c function\n");
    athread_init();
    athread_create(0, func, NULL);
    athread_wait(0);

    memcpy(arr, &sarr[0], 64000 * 8);
}
