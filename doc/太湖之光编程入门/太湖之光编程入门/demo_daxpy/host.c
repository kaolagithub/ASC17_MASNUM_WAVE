/* Author        : huangh223
 * Last modified : 2017-02-02
 */

#include <stdlib.h>
#include <stdio.h>
#include <athread.h>
#include <simd.h>
#include "my_slave.h"

static inline unsigned long rpcc()
{
    unsigned long time;
    asm("rtc %0": "=r" (time) : );
    return time;
}

int main()
{
    int correct = 1, N = 123456;

    double *x      = (double*) malloc(sizeof(double) * N);
    double *host_y = (double*) malloc(sizeof(double) * N);
    double *slave_y  = (double*) malloc(sizeof(double) * N);
    double alpha = 3.0;
    for (int i = 0; i < N; i++) 
    {
	x[i]       = 2.0;
	host_y[i]  = 1.0;
	slave_y[i] = 1.0;
    }

    struct slave_daxpy_param param;
    param.x = x;
    param.y = slave_y;
    param.alpha = alpha;
    param.size = N;

    athread_init();

    unsigned long st = rpcc();
    athread_spawn(daxpy, (void*)(&param));
    athread_join();
    unsigned long ed = rpcc();

    doublev4 *v4x = (doublev4*) &x[0];
    doublev4 *v4y = (doublev4*) &host_y[0];
    doublev4 v4alpha = alpha;
    int div4end = N / 4;
    int mod4end = div4end * 4;
    for (int i = 0; i < div4end; i++)
	v4y[i] += v4alpha * v4x[i];
    for (int i = mod4end; i < N; i++)
	host_y[i] += alpha * x[i];

    for (int i = 0; i < N; i++) 
	if (slave_y[i] != host_y[i])
	{
	    printf("Error at id %d : %lf <--> %lf\n", i, host_y[i], slave_y[i]);
	    correct = 0;
	}

    printf("Demo time = %e (s), correct = %d\n", ed - st, correct);
    free(x);
    free(slave_y);
    free(host_y);

    return 0;
}
