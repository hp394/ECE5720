/*****************************************************
 * this file can be compiled by using
 * gcc -o hp394_mm_rbyc hp394_mm_rbyc.c -lrt
 * then you can execute it by using
 * ./hp394_mm_tile a
 * a is the dimension of the matrix
 * if you don't provide a, the program will apply the default value
 * if you provide more than 1 value, only the first one will be used
 * I highly recommond to use the ./benchmark and it will compiled all the files
 * and writing the information into the corresponding csv file.
 *****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define BILLION 1000000000L
/**
 * generate Matiric according to the dimension
 */
int** generateMatrix(int N, int random) {
	int** A = malloc(sizeof(int*) * N);
	for (int i = 0; i < N; i++) {
		A[i] = malloc(sizeof(int) * N);
		for (int j = 0; j < N; j++) {
			if(random == 1)
				A[i][j] = drand48() * 100;
			else
				A[i][j] = 0;
		}
	}
	return A;
}

/**
 * print the Matrix used for test
 */
void print_matrix(int** A, int N) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%d ", A[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char* argv[]) {
  //set the default dimension size
	int N = 5;
	srand48(1);
  //get the dimension from the command line
	if(argc > 1) {
		N = atoi(argv[1]);
	}
	uint64_t diff;
	struct timespec start, end;
  //initialize the matrix
	int** A = generateMatrix(N, 1);
	int** B = generateMatrix(N, 1);
	int** C = generateMatrix(N, 0);
	
	clock_gettime(CLOCK_MONOTONIC, &start);
  //calculate the multiply result of two matrix row by column
	for (int i = 0; i < N; i++) {
		int* temp_A = A[i];
		for (int j = 0; j < N; j++) {
			int temp_C = C[i][j];
			for (int k = 0; k < N; k++) {
				temp_C += temp_A[k] * B[k][j];
			}
			C[i][j] = temp_C;
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &end);

	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;


  //writing the dimension and the elapsed time into a file
  FILE *fp;
  fp = fopen("rbyc-elapsed-time.csv", "a+");

  fprintf(fp, "%d,%llu\n", N, diff);

  fclose( fp );
	return 0;
}
