#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define BILLION 1000000000L
//#pragma comment(lib, "pthreadVC2.lib")
int** A;
int** B;
int** C;
//int** res;
int thread_size;
int N;
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
void init(int N) {
	srand48(1);
	A = malloc(sizeof(int*) * N);
	B = malloc(sizeof(int*) * N);
	C = malloc(sizeof(int*) * N);
//	res = malloc(sizeof(int*) * N);
	for (int i = 0; i < N; i++) {
		A[i] = malloc(sizeof(int) * N);
		B[i] = malloc(sizeof(int) * N);
		C[i] = malloc(sizeof(int) * N);
		//res[i] = malloc(sizeof(int) * N);
		for (int j = 0; j < N; j++) {
				A[i][j] = drand48() * 100;
				B[i][j] = drand48() * 100;
				C[i][j] = 0;
		//		res[i][j] = 0;
		}
	}
}
void print_matrix(int** A, int N) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%d ", A[i][j]);
		}
		printf("\n");
	}
}
int main(int argc, char* argv[]) {
	int thread_count = 2;
	N = 5;
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

	pthread_t* thread = malloc(thread_count * sizeof(pthread_t));
	thread_size = N / thread_count;
	int left_size = N % thread_count;
	uint64_t diff;
	struct timespec start, end;
	init(N);
	
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (int i = 0; i < thread_count; i++) {
		pthread_create(&thread[i], NULL,thread_mul_matrix, (void *)i);
	}
	for (int i = N - left_size; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++)
				C[i][j] += A[i][k] * B[k][j];
		}
	}

	for (int i = 0; i < thread_count; i++) {
		pthread_join(thread[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
//	printf("elapsed time of pthread is %llu ns\n", (long long unsigned int)diff);

	free(thread);
	free(A);
	free(B);
	free(C);
  
  FILE *fp;
  fp = fopen("pt-elapsed-time.csv", "a+");

  fprintf(fp, "%d,%d,%llu\n", N, thread_count, diff);

  fclose( fp );
	return 0;
}
