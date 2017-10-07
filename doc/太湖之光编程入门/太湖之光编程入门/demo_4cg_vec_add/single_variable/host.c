#include <stdlib.h>
#include <stdio.h>
#include <athread.h>
#include "pthread.h"
#include "simd.h"
#include "memory.h"

#define N 1024
extern SLAVE_FUN(func)();
double host_a[N];

void pfunc(void *tidptr)
{
    int tid = *((int*)tidptr);
    int athread_init_rv = athread_init();
    printf("Thread %d say hello, athread_init res = %d\n", tid, athread_init_rv);
    athread_spawn(func, (void*)&tid);
    athread_join();
}

int main()
{
	int correct = 1;
	pthread_t tids[4];
	int tid[4] = {0, 1, 2, 3};
	int main_id = 0;
	
	for (int i = 0; i < N; i++) host_a[i] = -1.0;

	
	for (int i = 1; i < 4; i++)
		pthread_create(&tids[i], NULL, &pfunc, &tid[i]);
	tids[0] = pthread_self();
	pfunc(&main_id);
	
	for (int i = 1; i < 4; i++)
		pthread_join(tids[i], NULL);

	for (int i = 0; i < N; i++) 
		printf("%d : %lf\n", i, host_a[i]);
	
	return 0;
}
