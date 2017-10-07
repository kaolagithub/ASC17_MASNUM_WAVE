/* Author        : huangh223
 * Last modified : 2017-02-02
 *
 * Notice : DO NOT rename this header file as "slave.h", since there
 * already has a "slave.h" in the SWCC and the slave program need to 
 * include that header file.
 *
 * This file aims to provide function interface information for the
 * host program. I hate writing "extern" keyword...
 */

#ifndef MY_SLAVE_H
#define MY_SLAVE_H

void slave_daxpy(void *_host_a);

struct slave_daxpy_param
{
    double *x, *y, alpha;
    int size;
};

#endif
