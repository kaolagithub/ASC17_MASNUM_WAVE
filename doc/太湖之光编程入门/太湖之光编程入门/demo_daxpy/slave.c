/* Author        : huangh223
 * Last modified : 2017-02-02
 *
 * Simply athread daxpy(Double Alpha * X Plus Y) program.
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <simd.h>
#include <slave.h>
#include "my_slave.h"

__thread_local volatile unsigned long get_reply, put_reply;

int block_low(const int N, const int total_blocks, const int block_i)
{
    return N * block_i / total_blocks;
}

int block_size(const int N, const int total_blocks, const int block_i)
{
    return block_low(N, total_blocks, block_i + 1) - block_low(N, total_blocks, block_i);
}


void slave_daxpy(void *_param)
{
    int my_id = athread_get_id(-1);
    struct slave_daxpy_param param = *((struct slave_daxpy_param*) _param);
    int seg_spos  = block_low (param.size, 64, my_id);
    int seg_size  = block_size(param.size, 64, my_id);
    int pad4v_seg_size = ((seg_size / 4) + 1) * 4;
    double *ldm_x = (double*) ldm_malloc(sizeof(double) * pad4v_seg_size);
    double *ldm_y = (double*) ldm_malloc(sizeof(double) * pad4v_seg_size);

    // printf(" [%d : %d, %d] ", my_id, seg_spos, seg_size);

    get_reply = 0;
    athread_get(PE_MODE, &param.x[seg_spos], &ldm_x[0], sizeof(double) * seg_size, (void*)(&get_reply), 0, 0, 0);
    athread_get(PE_MODE, &param.y[seg_spos], &ldm_y[0], sizeof(double) * seg_size, (void*)(&get_reply), 0, 0, 0);
    doublev4 v4alpha = param.alpha;
    while (get_reply != 2); 

    int div4end = seg_size / 4;
    int mod4end = div4end * 4;
    doublev4 *v4x = (doublev4*)(&ldm_x[0]);
    doublev4 *v4y = (doublev4*)(&ldm_y[0]);
    for (int i = 0; i < div4end; i++)
	v4y[i] += v4alpha * v4x[i];
    for (int i = mod4end; i < seg_size; i++)
	ldm_y[i] += param.alpha * ldm_x[i];

    put_reply = 0;
    athread_put(PE_MODE, &ldm_x[0], &param.x[seg_spos], sizeof(double) * seg_size, (void*)(&put_reply), 0, 0);
    athread_put(PE_MODE, &ldm_y[0], &param.y[seg_spos], sizeof(double) * seg_size, (void*)(&put_reply), 0, 0);
    while (put_reply != 2);
}
