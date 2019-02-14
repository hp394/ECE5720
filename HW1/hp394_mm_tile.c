#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int** generateMatrix(int N, int random) {
	int** A = malloc(sizeof(int*) * N);
	for (int i = 0; i < N; i++) {
		A[i] = malloc(sizeof(int) * N);
		for (int j = 0; j < N; j++) {
			if (random == 1)
				A[i][j] = rand() % 1000;
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
int main() {
	int N = 6;
	int tile = 2;
	srand(time(NULL));
	int** A = generateMatrix(N, 1);
	int** B = generateMatrix(N, 1);
	int** C = generateMatrix(N, 0);
	int** res = generateMatrix(N, 0);
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
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++)
				res[i][j] += A[i][k] * B[k][j];
	print_matrix(C, N);
	printf("\n");
	print_matrix(res, N);



	//printf("hello,world!");
	return 0;
}