#include <stdio.h>
#include <math.h>
#include <string.h>
#include <simd.h>
#include <slave.h>

#define TN 1000
#define N (256 * 4 * TN)

__thread_local volatile unsigned long get_reply, put_reply;
__thread_local doublev4 ldm_a[TN]; 
extern double host_a[N];

void func(void *_cgid)
{
	int my_id = athread_get_id(-1);
	int my_cg = *((int*)_cgid);
	int my_spos = (64 * my_cg + my_id) * 4 * TN;
	double d_my_cg = my_cg;
	doublev4 v4_my_cg = d_my_cg; 
	
	get_reply = 0;
	int get_res = athread_get(PE_MODE, &host_a[my_spos], &ldm_a[0], sizeof(double) * 4 * TN, (void*)(&get_reply), 0, 0, 0);
	while (get_reply != 1); 
	
	for (int i = 0; i < TN; i++)
		ldm_a[i] += v4_my_cg;
	
	put_reply = 0;
	int put_res = athread_put(PE_MODE, &ldm_a[0], &host_a[my_spos], sizeof(double) * 4* TN, (void*)(&put_reply), 0, 0);
	while (put_reply != 1);
}