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
			if (random == 1)
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
	int N = 6;
	int tile = 2;
	if(argc > 2) {
		N = atoi(argv[1]);
		tile = atoi(argv[2]);
		if(tile == 0 && N != 0) {
			printf("the tile size cannot be zero! \n");
			return -1;
		}
		if(N % tile != 0) {
			printf("the dimension and the tile size are not matched! \n");
			return -1;
		} 
	} else if(argc == 2) {
			printf("missing parameters! \n");
			return -1;
	}
	srand48(1);
	
	uint64_t diff;
	struct timespec start, end;
	int** A = generateMatrix(N, 1);
	int** B = generateMatrix(N, 1);
	int** C = generateMatrix(N, 0);
//	int** res = generateMatrix(N, 0);
	
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (int i = 0; i < N; i += tile) {
		for (int j = 0; j < N; j += tile) {
			for (int k = 0; k < N; k += tile) {
				for (int i1 = i; i1 < i + tile; i1++)
					for (int j1 = j; j1 < j + tile; j1++)
						for (int k1 = k; k1 < k + tile; k1++)
							C[i1][j1] += A[i1][k1] * B[k1][j1];
			}

		}
	}
	
	clock_gettime(CLOCK_MONOTONIC, &end);
	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	printf("elapsed time of tile is %llu ns\n", (long long unsigned int)diff);




	return 0;
}
