#include <stdlib.h>
#include <stdio.h>
#include <athread.h>
#include <pthread.h>

#define TN 1000
#define N (256 * 4 * TN)
const int CGN = N / 4;

extern SLAVE_FUN(func)();
double host_a[N];

void pfunc(void *tidptr)
{
    int tid = *((int*)tidptr);
    athread_init();
    athread_spawn(func, (void*)&tid);
    athread_join();
}

int main()
{
	int correct = 1;
	pthread_t tids[4];
	int tid[4] = {0, 1, 2, 3};
	int main_id = 0;
	
	// 初始化 host_a 的值
	for (int i = 0; i < N; i++) host_a[i] = 0.0;

	// 创建 3 条 pthread 进程
	// 注意，不能 fork 4 条 pthread 出来，否则运行时会报错
	for (int i = 1; i < 4; i++)
		pthread_create(&tids[i], NULL, &pfunc, &tid[i]);
	tids[0] = pthread_self();
	pfunc(&main_id);
	
	// 等待创建的线程执行完毕
	for (int i = 1; i < 4; i++)
		pthread_join(tids[i], NULL);

	// 将 host_a 的值减去它们『应该』被 athread 加上的值
	for (int k = 0; k < 4; k++)
	{
		double dk = k;
		for (int i = 0; i < CGN; i++)
			host_a[k * CGN + i] -= dk;
	}
	
	// 检查各值是否为初始值
	for (int i = 0; i < N; i++) 
		if (host_a[i] != 0.0)
		{
			printf("No. %d : %lf, should be 0.0\n", i, host_a[i]);
			correct = 0;
			break;
		}
	
	printf("Correct = %d\n", correct);
	
	return 0;
}
