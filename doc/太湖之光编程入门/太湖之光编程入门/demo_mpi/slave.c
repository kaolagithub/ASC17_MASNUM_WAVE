#include <stdio.h>
#include <math.h>
#include <string.h>
#include "simd.h"
#include "slave.h"
#include "dma.h"

#ifndef __thread_local
//just for obtaining better look on some IDE with error hints
#define __thread_local const
#endif

#define J 64
#define I 2000
#define K 4
__thread_local volatile unsigned long get_reply,put_reply;
__thread_local volatile unsigned long start,end;
__thread_local int my_id, my_core;
__thread_local double c_slave[I];
__thread_local double a_slave[1000];
__thread_local long trump[100];

extern double c[K][J][I];
extern double a[6400];

void func()
{
    dma_desc dmad_put, dmad_get, dmad_barrier;
    int i,j,k;
    int rev;
    my_id = athread_get_id(-1);
    my_core = athread_get_core(-1);
    int dma_mask = 0xFFFFFFFF;

    dma_set_op(&dmad_get, DMA_GET);
    dma_set_mode(&dmad_get, BCAST_MODE);
    dma_set_size(&dmad_get, 8 * 24);
    dma_set_reply(&dmad_get, &get_reply);
    dma_set_mask(&dmad_get, dma_mask);

    long stcc, edcc;

    RPCC(stcc);

    int lpc;
    lpc = 128 - my_id;
    for(i = 0; i < 100; i++)
        trump[i] = (i + 8) % 100;
    k = 0;
    for(i = 0; i < 1000 * lpc; i++)
        for(j = 0; j < 100; j++)
            k = trump[trump[k]];

    int total_mask = 0xFFFFFFFF;
    get_reply = 0;
    if(my_id == 0)
    {
        get_reply = 0;
        dma(dmad_get, &a[0], &a_slave[0]);
        while(get_reply!=1);
    }
    rev = athread_syn(ARRAY_SCOPE, total_mask);

    RPCC(edcc);

    //make sure that 0 goes the last
    for(i = 0; i < 1000 * lpc; i++)
        for(j = 0; j < 100; j++)
            k = trump[trump[k]];

    for(i = 0; i < I; i+=3)
    {
        c_slave[i] = my_id;
        c_slave[i + 1] = my_core;
        c_slave[i + 2] = (edcc - stcc + i + k) * 1.0;
    }

    for(i = 0; i < 10; i++)
        c_slave[3 + i] = a_slave[i] + 10 * my_id;

    put_reply = 0;

    dma_set_op(&dmad_put, DMA_PUT);
    dma_set_mode(&dmad_put, PE_MODE);
    dma_set_size(&dmad_put, I * 4);
    dma_set_reply(&dmad_put, &put_reply);

    dma(dmad_put, &c[0][my_id][0], &c_slave[0]);

    while(put_reply!=1);
}
