#include <stdlib.h>
#include <stdio.h>
#include <athread.h>
#include <fcntl.h>
#include "simd.h"
#include "memory.h"
#include <unistd.h>
#include <mpi.h>
#include "inttypes.h"

extern SLAVE_FUN(func)();

#define J 64
#define I 2000
#define K 4

#define mcw MPI_COMM_WORLD

double c[K][J][I];
double a[6400];
long a_paddr, a_paddr2;

uintptr_t vtop(uintptr_t vaddr)
{
    FILE *pagemap;
    intptr_t paddr = 0;
    int offset = (vaddr / sysconf(_SC_PAGESIZE)) * sizeof(uint64_t);
    uint64_t e;

    if ((pagemap = fopen("/proc/self/pagemap", "r")))
    {
        if (lseek(fileno(pagemap), offset, SEEK_SET) == offset)
        {
            if (fread(&e, sizeof(uint64_t), 1, pagemap))
            {
                if (e & (1ULL << 63))
                { // page present ?
                    paddr = e & ((1ULL << 54) - 1); // pfn mask
                    paddr = paddr * sysconf(_SC_PAGESIZE);
                    // add offset within page
                    paddr = paddr | (vaddr & (sysconf(_SC_PAGESIZE) - 1));
                }
            }
        }
        fclose(pagemap);
    }
    return paddr;
}

void MPIParaMission(int my_rank)
{
    a_paddr = vtop(&a[0]);
    int dst = my_rank ^ 0x00000001;
    MPI_Sendrecv(&a_paddr, 1, MPI_DOUBLE, dst, 0, &a_paddr2, 1, MPI_DOUBLE, dst, 0, mcw, MPI_STATUS_IGNORE);
    a_paddr = a_paddr2;

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

    MPIParaMission(my_rank);

    for(i = 0; i < comm_sz; i++)
    {
        if(i == my_rank)
            MPISerialFunc(my_rank);
        MPI_Barrier(mcw);
    }


    MPI_Finalize();

    return 0;
}