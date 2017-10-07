#include <stdio.h>
#include <athread.h>
#include <sys/time.h>

double a[4];

#define FREQ_SEC 1450000000
#define FREQ_USEC 1450

extern SLAVE_FUN(func_df36)();
extern SLAVE_FUN(func_df37)();

static inline unsigned long rpcc()
{
    unsigned long time;
    asm("rtc %0": "=r" (time) : );
    return time;
}

int main()
{
    struct timeval starttime, endtime;
    double timeuse;
    double rate;
    athread_init();

    gettimeofday(&starttime,0);
    athread_create(0, func_df36, NULL);
    athread_wait(0);
    gettimeofday(&endtime,0);
    timeuse = 1000000*(endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
    rate = FREQ_USEC * timeuse;
    rate = 1260000000 / rate;
    rate *= 100.0;
    printf("i = 6, sysclock = %.2f us, %.4f%% peak\n", timeuse, rate);


    long stcc = rpcc();
    gettimeofday(&starttime,0);
    athread_create(0, func_df37, NULL);
    athread_wait(0);
    gettimeofday(&endtime,0);
    timeuse = 1000000*(endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
    rate = FREQ_USEC * timeuse;
    rate = 1260000000 / rate;
    rate *= 100.0;
    stcc = rpcc() - stcc;
    printf("i = 7, sysclock = %.2f us, %.4f%% peak\n", timeuse, rate);

    return 0;
}