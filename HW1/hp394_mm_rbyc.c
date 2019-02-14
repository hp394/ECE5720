#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int** generateMatrix(int N, int random) {
	int** A = malloc(sizeof(int*) * N);
	for (int i = 0; i < N; i++) {
		A[i] = malloc(sizeof(int) * N);
		for (int j = 0; j < N; j++) {
			if(random == 1)
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
	int N = 5;
	srand(time(NULL));
	int** A = generateMatrix(N, 1);
	int** B = generateMatrix(N, 1);
	int** C = generateMatrix(N, 0);
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
	print_matrix(C, N);



	//printf("hello,world!");
	return 0;
}