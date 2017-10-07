#include <stdio.h>
#include <math.h>
#include <string.h>
#include <simd.h>
#include <slave.h>

#define N 1024

__thread_local volatile unsigned long get_reply, put_reply;
extern double host_a[N];

void func(void *_cgid)
{
	int my_id = athread_get_id(-1);
	int my_cg = *((int*)_cgid);
	int my_spos = (64 * my_cg + my_id) * 4;
	double d_my_cg = my_cg;
	volatile doublev4 ldm_a, v4_my_cg = d_my_cg; // 很奇怪，开 -O2 的话这里不加 volatile 会出错
	
	get_reply = 0;
	int get_res = athread_get(PE_MODE, &host_a[my_spos], &ldm_a, sizeof(double) * 4, (void*)(&get_reply), 0, 0, 0);
	while (get_reply != 1); 
	
	ldm_a += v4_my_cg;
	
	put_reply = 0;
	int put_res = athread_put(PE_MODE, &ldm_a, &host_a[my_spos], sizeof(double) * 4, (void*)(&put_reply), 0, 0);
	while (put_reply != 1);
}
