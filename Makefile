all:
	mkoctfile -v -I-fopenmp hist_omp.cpp
clean:
	rm hist_omp.oct hist_omp.o
