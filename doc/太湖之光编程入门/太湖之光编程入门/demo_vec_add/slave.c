/* Author		: huangh223
 * Last modified : 2017-02-02
 *
 * Simply athread test program. The slave_func will get the data
 * from the host memory, add 5 to each element, and write back to 
 * the host memory. The address of the target array is provided 
 * by the only paramater of the function.
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <simd.h>
#include <slave.h>
#include "my_slave.h"

#define N 10

__thread_local volatile unsigned long get_reply, put_reply;
__thread_local double recv_a[N];

void slave_func(void *_host_a)
{
	int my_id = athread_get_id(-1);
	// The convertion in the next line is highly recommended, otherwise 
	// following operations may have unknown errors.
	double *host_a = (double*)(_host_a); 

	/* WARNING! WARNING! WARNING!
	 * The "athread_get" and "athread_set" functions are ASYNC! NOT SYNC!
	 * To ensure that your data is correctly fetched or sent, you should reset
	 * the reply flag variable (the 5th paramter) and wait the operation.
	 *
	 * This important inforation is not mentioned in <<太湖之光基础语言编译手册>>
	 * ver 20140801. But you can find the clues in the <<太湖之光快速使用手册>>.
	 * ver 1.0 (ver 20160815). What the f**k...
	 */
	// Reset reply flag
	get_reply = 0;
	// Transmission data
	int get_res = athread_get(PE_MODE, &host_a[my_id * N], &recv_a[0], sizeof(double) * N, (void*)(&get_reply), 0, 0, 0);
	// If you call %k% athread_get after you reset the reply flag, these %k% operations are completed 
	// when the value of the reply flag equals to %k%. Using athread_put is the same as using athread_get.
	while (get_reply != 1); 

	doublev4 *v4a = (doublev4*)(&recv_a[0]);
	doublev4 _5   = 5.0;
	int div4end   = N / 4;
	int mod4end   = div4end * 4;
	// Vectorized add
	for (int i = 0; i < div4end; i++) v4a[i] += _5;
	/* An more explicit way:
	doublev4 v4a0;
	for (int i = 0; i < mod4end; i += 4)
	{
		simd_load(v4a0, &recv_a[i]);
		v4a0 += _5;
		simd_store(v4a0, &recv_a[i]);
	}
	*/
	for (int i = mod4end; i < N; i++) recv_a[i] += 5.0;
	
	put_reply = 0;
	int put_res = athread_put(PE_MODE, &recv_a[0], &host_a[my_id * N], sizeof(double) * N, (void*)(&put_reply), 0, 0);
	while (put_reply != 1);
}
