#include <stdio.h>
#include <math.h>
#include <string.h>
#include "simd.h"
#include "slave.h"

#define FUNC3(a, b, c) __builtin_sw64_vmad(a, b, c)

__thread_local volatile unsigned long put_reply;
__thread_local doublev4 va = {1.0, 2.0, 3.0, 0.0}, vb = {4.0, 3.0, 2.0, 1.0}, \
 vaa = {2.0, 3.0, 4.0, 5.0}, vbb = {5.0, 4.0, 3.0, 2.0}, vab = {2.1, 2.3, 2.5, 2.7}, \
 va1 = {3.1, 3.2, 3.3, 3.4}, va2 = {3.4, 1.1, 3.7, 2.4}, va3 = {5.1, 2.2, 2.3, 1.4}, \
 va4 = {1.4, 1.7, 1.85, 2.13}, va5 = {1.43, 1.73, 1.8512, 2.131}, vxx = {0, 0.81, 0.94, 0.92}, vx = {0.91, 0, 0.93, 0.94},  vc = {1.1, 1.2, 1.3, 1.4};

extern double a[];

void func_df36()
{
    int i;

    for(i = 0; i < 105000000; i++)
    {
        va = FUNC3(va, vx, vc);
        vb = FUNC3(vb, vx, vc);
        vaa = FUNC3(vaa, vx, vc);
        vbb = FUNC3(vbb, vx, vc);
        vab = FUNC3(vab, vx, vc);
        va1 = FUNC3(va1, vx, vc);
        va = FUNC3(va, vx, vc);
        vb = FUNC3(vb, vx, vc);
        vaa = FUNC3(vaa, vx, vc);
        vbb = FUNC3(vbb, vx, vc);
        vab = FUNC3(vab, vx, vc);
        va1 = FUNC3(va1, vx, vc);
    }

    va = va + vb + vaa + vbb + vab + va1;

    put_reply = 0;
    athread_put(PE_MODE, &va, &a[0], 4 * sizeof(double), &put_reply, 0, 0);
    
    while(put_reply!=1);
}

void func_df37()
{
    int i;

    for(i = 0; i < 90000000; i++)
    {
        va = FUNC3(va, vx, vc);
        vb = FUNC3(vb, vx, vc);
        vaa = FUNC3(vaa, vx, vc);
        vbb = FUNC3(vbb, vx, vc);
        vab = FUNC3(vab, vx, vc);
        va1 = FUNC3(va1, vx, vc);
        va2 = FUNC3(va2, vx, vc);
        va = FUNC3(va, vx, vc);
        vb = FUNC3(vb, vx, vc);
        vaa = FUNC3(vaa, vx, vc);
        vbb = FUNC3(vbb, vx, vc);
        vab = FUNC3(vab, vx, vc);
        va1 = FUNC3(va1, vx, vc);
        va2 = FUNC3(va2, vx, vc);
    }

    va = va + vb + vaa + vbb + vab + va1 + va2;

    put_reply = 0;
    athread_put(PE_MODE, &va, &a[0], 4 * sizeof(double), &put_reply, 0, 0);
    
    while(put_reply!=1);
}