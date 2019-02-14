#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#pragma comment(lib, "pthreadVC2.lib")
int** A;
int** B;
int** C;
int** res;
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
	srand(time(NULL));
	A = malloc(sizeof(int*) * N);
	B = malloc(sizeof(int*) * N);
	C = malloc(sizeof(int*) * N);
	res = malloc(sizeof(int*) * N);
	for (int i = 0; i < N; i++) {
		A[i] = malloc(sizeof(int) * N);
		B[i] = malloc(sizeof(int) * N);
		C[i] = malloc(sizeof(int) * N);
		res[i] = malloc(sizeof(int) * N);
		for (int j = 0; j < N; j++) {
				A[i][j] = rand() % 1000;
				B[i][j] = rand() % 1000;
				C[i][j] = 0;
				res[i][j] = 0;
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
int main() {
	int thread_count = 2;
	N = 5;
	pthread_t* thread = malloc(thread_count * sizeof(pthread_t));
	thread_size = N / thread_count;
	int left_size = N % thread_count;
	init(N);
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

	print_matrix(C, N);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++)
				res[i][j] += A[i][k] * B[k][j];
	printf("\n");
	print_matrix(res, N);
	
	free(thread);
	free(A);
	free(B);
	free(C);
	return 0;
}
