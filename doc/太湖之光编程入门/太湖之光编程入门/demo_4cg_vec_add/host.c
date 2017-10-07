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
	
	// ��ʼ�� host_a ��ֵ
	for (int i = 0; i < N; i++) host_a[i] = 0.0;

	// ���� 3 �� pthread ����
	// ע�⣬���� fork 4 �� pthread ��������������ʱ�ᱨ��
	for (int i = 1; i < 4; i++)
		pthread_create(&tids[i], NULL, &pfunc, &tid[i]);
	tids[0] = pthread_self();
	pfunc(&main_id);
	
	// �ȴ��������߳�ִ�����
	for (int i = 1; i < 4; i++)
		pthread_join(tids[i], NULL);

	// �� host_a ��ֵ��ȥ���ǡ�Ӧ�á��� athread ���ϵ�ֵ
	for (int k = 0; k < 4; k++)
	{
		double dk = k;
		for (int i = 0; i < CGN; i++)
			host_a[k * CGN + i] -= dk;
	}
	
	// ����ֵ�Ƿ�Ϊ��ʼֵ
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
