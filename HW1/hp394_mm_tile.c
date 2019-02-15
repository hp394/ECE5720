/*****************************************************
 * this file can be compiled by using
 * gcc -o hp394_mm_tile hp394_mm_tile.c -lrt
 * then you can execute it by using
 * ./hp394_mm_tile a b
 * a is the dimension of the matrix and b is the tile size
 * if you don't provide a and b, the program will apply the default value
 * if you provide more than 2 value, only the first two will be used
 * if you only provide one value, there would be an error.
 * if the dimension cannot be divisible by tile_size, there would be an error
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
			if (random == 1)
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
	int N = 6;
  //set the default tile size
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
  //initialize the Matrix
	int** A = generateMatrix(N, 1);
	int** B = generateMatrix(N, 1);
	int** C = generateMatrix(N, 0);
	
	clock_gettime(CLOCK_MONOTONIC, &start);
  //traverse all the tiles
	for (int i = 0; i < N; i += tile) {
		for (int j = 0; j < N; j += tile) {
			for (int k = 0; k < N; k += tile) {
        //multiply the tile
				for (int i1 = i; i1 < i + tile; i1++)
					for (int j1 = j; j1 < j + tile; j1++)
						for (int k1 = k; k1 < k + tile; k1++)
							C[i1][j1] += A[i1][k1] * B[k1][j1];
			}

		}
	}
	
	clock_gettime(CLOCK_MONOTONIC, &end);
  //calculate the elapsed time
	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;

  
  //write the dimension size, tile size and elapsed time into a file
  FILE *fp;
  fp = fopen("tile-elapsed-time.csv", "a+");

  fprintf(fp, "%d,%d,%llu\n", N, tile, diff);

  fclose( fp );

	return 0;
}
