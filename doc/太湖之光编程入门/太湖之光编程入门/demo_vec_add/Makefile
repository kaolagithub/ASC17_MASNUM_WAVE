# SWCC project makefile, test version
# Author        : huangh223
# Last modifued : 2017-02-02

EXE	= test
CC	= mpicc
SCC	= sw5cc
HFLAGS	= -host  -std=gnu99 -O2 -msimd 
SFLAGS	= -slave -std=gnu99 -O2 -msimd
# For slave functions, "-slave" is required, "-msimd" is required if you want to 
# use vectorization. "-std=gnu99" allows us to write "for (int i = ...". 
LDFLAGS	=

test:host.o slave.o
	$(CC) $(LDFLAGS) *.o -o $@

host.o: host.c
	$(CC) -c $(HFLAGS) host.c

slave.o: slave.c
	$(SCC) -c $(SFLAGS) slave.c

clean:
	rm *.o
