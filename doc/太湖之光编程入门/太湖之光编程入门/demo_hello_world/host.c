#include <athread.h>
#include "pthread.h"
#include <stdio.h>

extern SLAVE_FUN(func)();

static inline unsigned long rpcc()
{
    unsigned long time;
    asm("rtc %0": "=r" (time) : );
    return time;
}

int main()
{
    unsigned long st, ed;

    athread_init();

    st = rpcc();
    athread_spawn(func, NULL);
    athread_join();
    ed=rpcc();

    printf("SYSU ASC17 demo costs %ld clock cycles\n", ed - st);

    return 0;
}
