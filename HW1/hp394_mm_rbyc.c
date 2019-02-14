#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define BILLION 1000000000L
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

void print_matrix(int** A, int N) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%d ", A[i][j]);
		}
		printf("\n");
	}
}
int main(int argc, char* argv[]) {
	int N = 5;
	srand48(1);
	if(argc > 1) {
		N = atoi(argv[1]);
	}
	uint64_t diff;
	struct timespec start, end;
	int** A = generateMatrix(N, 1);
	int** B = generateMatrix(N, 1);
	int** C = generateMatrix(N, 0);
	
	clock_gettime(CLOCK_MONOTONIC, &start);
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
	//print_matrix(C, N);
	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	printf("elapsed time of rbyc is %llu ns\n", (long long unsigned int)diff);


	//printf("hello,world!");
	return 0;
}
