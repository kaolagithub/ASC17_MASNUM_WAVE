#include <stdlib.h>
#include <stdio.h>
#include <athread.h>
#include <simd.h>

#define src_id 10

extern SLAVE_FUN(broadcast)();

static inline unsigned long rpcc()
{
    unsigned long time;
    asm("rtc %0": "=r" (time) : );
    return time;
}

int main()
{
    int correct = 1, N = 64 * 4;
	double src_value = src_id;

    double *bcast_res = (double*) malloc(sizeof(double) * N);
    for (int i = 0; i < N; i++) 
		bcast_res[i] = -1.0;

    athread_init();

    unsigned long st = rpcc();
    athread_spawn(broadcast, (void*)bcast_res);
    athread_join();
    unsigned long ed = rpcc();
	printf("Slave work done\n");

    for (int i = 0; i < N; i++) 
		if (bcast_res[i] != src_value)
		{
			printf("Error at id %d : %lf <--> %lf\n", i, bcast_res[i], src_value);
			correct = 0;
		}

    printf("Demo time = %e (s), correct = %d\n", ed - st, correct);
    free(bcast_res);

    return 0;
}
