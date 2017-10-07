EXE     = test
CC      = mpicc
FC      = mpif90
SCC     = sw5cc
FFLAGS  = -O2
HFLAGS  = -host  -msimd -O2 -std=gnu99
SFLAGS  = -slave -msimd -O2 -std=gnu99
LDFLAGS = -O2
SRC     = doge.f90 c_doge.c slave.c

$(EXE): doge.o c_doge.o slave.o
	$(FC) $(LDFLAGS) *.o  -o $@

doge.o: doge.f90
	$(FC) -c $(FFLAGS) doge.f90

c_doge.o: c_doge.c
	$(CC) -c $(HFLAGS) c_doge.c

slave.o: slave.c
	$(SCC) -c $(SFLAGS) slave.c

clean:
	rm *.o