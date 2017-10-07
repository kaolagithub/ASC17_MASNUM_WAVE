EXE	= test
CC	= mpicc
SCC	= sw5cc
HFLAGS	= -host  -std=gnu99 -O2 -msimd 
SFLAGS	= -slave -std=gnu99 -O2 -msimd
LDFLAGS	=

$(EXE):host.o slave.o
	$(CC) $(LDFLAGS) *.o -o $@

host.o: host.c
	$(CC) -c $(HFLAGS) host.c

slave.o: slave.c
	$(SCC) -c $(SFLAGS) slave.c

clean:
	rm *.o
