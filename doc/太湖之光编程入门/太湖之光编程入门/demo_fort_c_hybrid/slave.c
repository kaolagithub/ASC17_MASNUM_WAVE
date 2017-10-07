#include "slave.h"
#include "dma.h"
#include "stdio.h"

__thread_local double localarr[1000];
__thread_local volatile int dma_reply;

extern double sarr[64000];

void func()
{
    dma_desc dmad_put;
    int i, p;

    dma_reply = 0;

    dma_set_op(&dmad_put, DMA_PUT);
    dma_set_mode(&dmad_put, PE_MODE);
    dma_set_size(&dmad_put, 1000 * 8);
    dma_set_reply(&dmad_put, &dma_reply);

    for(p = 0; p < 64; p++)
    {
        for(i = 0; i < 1000; i++)
            localarr[i] = i + 1 + p * 1000;
        dma(dmad_put, &sarr[p * 1000], &localarr[0]);
    }

    /*for(i = 0; i < 1000000; i++)
        localarr[i % 3 + 500] -= localarr[(i + 1) % 3 + 500];
    printf("reply is %d\n", dma_reply);*///you can try how these three lines will behave with or without "volatile"
    while(dma_reply!=64);
}
