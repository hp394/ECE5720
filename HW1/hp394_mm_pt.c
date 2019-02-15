/*****************************************************
 * this file can be compiled by using
 * gcc -o hp394_mm_pt hp394_mm_pt.c -lrt -pthread
 * then you can execute it by using
 * ./hp394_mm_pt a b
 * a is the dimension of the matrix and b is the thread number
 * if you don't provide a and b, the program will apply the default value
 * if you provide more than 2 value, only the first two will be used
 * if you only provide one value, there would be an error
 * I highly recommond to use the ./benchmark and it will compiled all the files
 * and writing the information into the corresponding csv file.
 *****************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define BILLION 1000000000L

int** A;
int** B;
int** C;
int thread_size;
int N;

/**
 * matrix multiply inside one thread
 */
void *thread_mul_matrix(void* tid) {
	int id = (int)tid;
	int start_row = id * thread_size;
	int end_row = (id + 1)* thread_size;
	
	for(int i = start_row; i < end_row; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++)
				C[i][j] += A[i][k] * B[k][j];
		}
	}

	pthread_exit(NULL);
}

/**
 * initialize the matrix
 */
void init(int N) {
	srand48(1);
	A = malloc(sizeof(int*) * N);
	B = malloc(sizeof(int*) * N);
	C = malloc(sizeof(int*) * N);

	for (int i = 0; i < N; i++) {
		A[i] = malloc(sizeof(int) * N);
		B[i] = malloc(sizeof(int) * N);
		C[i] = malloc(sizeof(int) * N);

		for (int j = 0; j < N; j++) {
				A[i][j] = drand48() * 100;
				B[i][j] = drand48() * 100;
				C[i][j] = 0;
		}
	}
}

/**
 * print the matrix for test
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
  //set the default value of the matrix dimension and thread number
	int thread_count = 2;
	N = 5;
  //get the value of the matrix dimension and thread number from commond line
	if(argc > 2) {
		N = atoi(argv[1]);
		thread_count = atoi(argv[2]);
		thread_count = thread_count > N ? N : thread_count;
		thread_count = thread_count > 1 ? thread_count : 1;
		if(thread_count == 0 && N != 0){
			printf("wrong input! \n");
			return -1;
		}
	} else if(argc == 2) {
			printf("missing parameters! \n");
			return -1;
	}

  //initialize the thread and the matrix
	pthread_t* thread = malloc(thread_count * sizeof(pthread_t));
	thread_size = N / thread_count;
	int left_size = N % thread_count;
	uint64_t diff;
	struct timespec start, end;
	init(N);
	
	clock_gettime(CLOCK_MONOTONIC, &start);
  
  //start the thread
	for (int i = 0; i < thread_count; i++) {
		pthread_create(&thread[i], NULL,thread_mul_matrix, (void *)i);
	}
  //calculate multiply the left part of the matrix
	for (int i = N - left_size; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++)
				C[i][j] += A[i][k] * B[k][j];
		}
	}

  //wait until all the thread are finished
	for (int i = 0; i < thread_count; i++) {
		pthread_join(thread[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
  
  //calculate the elapsed time
	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;


	free(thread);
	free(A);
	free(B);
	free(C);
  
  //write the dimension size, thread number and elapsed time into a file
  FILE *fp;
  fp = fopen("pt-elapsed-time.csv", "a+");

  fprintf(fp, "%d,%d,%llu\n", N, thread_count, diff);

  fclose( fp );
	return 0;
}
