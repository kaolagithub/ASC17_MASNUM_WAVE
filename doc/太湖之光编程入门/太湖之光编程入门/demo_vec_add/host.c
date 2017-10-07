/* Author		: huangh223
 * Last modified : 2017-02-02
 */

#include <stdlib.h>
#include <stdio.h>
#include <athread.h>
#include "my_slave.h"

#define N 10

static inline unsigned long rpcc()
{
	unsigned long time;
	asm("rtc %0": "=r" (time) : );
	return time;
}

int main()
{
	double *host_a;
	unsigned long st, ed;
	int i, correct;

	correct = 1;
	host_a = (double*) malloc(sizeof(double) * N * 64);
	for (i = 0; i < N * 64; i++) host_a[i] = 1.0;

	athread_init();

	st = rpcc();
	athread_spawn(func, (void*)(host_a));
	athread_join();
	ed = rpcc();

	for (i = 0; i < N * 64; i++) 
	if (host_a[i] != 6.0)
	{
		printf("Error at id %d : %lf\n", i, host_a[i]);
		correct = 0;
	}

	printf("Demo time = %e (s), correct = %d\n", ed - st, correct);
	free(host_a);

	return 0;
}
