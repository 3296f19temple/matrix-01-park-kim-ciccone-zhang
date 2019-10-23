PGMS=mmult_omp_timing automation nParammult matrix_times_vector hello pi mxv_omp_mpi mmult_mpi_omp

all:	${PGMS}

mmult_mpi_omp:		mmult.o mmult_mpi_omp.o
	mpicc -o mmult_mpi_omp -fopenmp -O3 mmult.o mmult_mpi_omp.o

mmult_mpi_omp.o:	mmult_mpi_omp.c
	mpicc -c -fopenmp -O3 mmult_mpi_omp.c

mmult_omp_timing:	mmult.o mmult_omp.o mmult_omp_timing.o
	gcc -o mmult -fopenmp -O3 mmult.o mmult_omp.o mmult_omp_timing.o -o mmult_omp_timing

mmult.o:	mmult.c
	gcc -c -O3 mmult.c

nParammult:     nParammult.c
	gcc -o nParammult nParammult.c
	mpicc -03 -o SIMDmult nParammult.c

automation:     automation.c
	gcc -o automation automation.c

mmult_omp.o:	mmult_omp.c
	gcc -c -O3 -fopenmp mmult_omp.c

mmult_omp_timing.o:	mmult_omp_timing.c
	gcc -c -O3 mmult_omp_timing.c

matrix_times_vector:	matrix_times_vector.c
	mpicc -O3 -o matrix_times_vector matrix_times_vector.c

hello:	hello.c
	mpicc -O3 -o hello hello.c

pi:	pi.c
	mpicc -O3 -o pi pi.c

mxv_omp_mpi:	mxv_omp_mpi.c
	mpicc -fopenmp -O3 -o mxv_omp_mpi mxv_omp_mpi.c

clean:
	rm -f *.o
	rm -r Data
	rm -f output.txt
	rm -f ${PGMS}

