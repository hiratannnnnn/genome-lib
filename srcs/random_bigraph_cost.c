#include "lib.h"

double **gen_rand_bigraph_cost(int a, int b)
{
	double **matrix;
	int i, j;

	matrix = gen_matrix_double(a, b);
	if (!matrix)
		return NULL;
	for (i = 0; i < a; i++)
		for (j = 0; j < b; j++)
			matrix[i][j] = (double)rand() / RAND_MAX;
	return matrix;
}
