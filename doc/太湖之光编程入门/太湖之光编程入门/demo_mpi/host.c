#include <stdlib.h>
#include <stdio.h>
#include <athread.h>
#include <fcntl.h>
#include "simd.h"
#include "memory.h"
#include <unistd.h>
#include <mpi.h>

extern SLAVE_FUN(func)();

#define J 64
#define I 2000
#define K 4

#define mcw MPI_COMM_WORLD

double c[K][J][I];
double a[6400];

void MPIParaMission()
{
    athread_init();
    athread_spawn(func, NULL);
    athread_wait(0);

}

void MPISerialFunc(int my_rank)//this func will be excuted serial, guaranteed by main function.
{
    int i, j, k;
    for(i = 0; i < 1; i++)
        for(j = 0; j < 64; j++)
            for(k = 2; k < 7; k++)
            {
                printf("%d, %d, %d, %.1f\n", my_rank, j, k, c[i][j][k]);fflush(stdout);
            }
}

int main()
{
    MPI_Init(NULL, NULL);

    int my_rank, comm_sz, namelen;
    MPI_Comm_rank(mcw, &my_rank);
    MPI_Comm_size(mcw, &comm_sz);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(processor_name,&namelen);

    printf("Hello world from proc %d of %d, processor name = %s\n", my_rank, comm_sz, processor_name);

    int i, j, k;

    for(i = 0; i < 6400; i++)
        a[i] = 1.0 * i + 10000 * my_rank;

    MPIParaMission();

    for(i = 0; i < comm_sz; i++)
    {
        if(i == my_rank)
            MPISerialFunc(my_rank);
        MPI_Barrier(mcw);
    }


    MPI_Finalize();

    return 0;
}