#include "lib.h"

int **gen_rand_bigraph(int a, int b, double edge_prob)
{
	int **matrix;
	int i, j;

	matrix = gen_matrix_int(a, b);
	if (!matrix)
		return NULL;

	if (edge_prob <= 0.0)
		return matrix;

	for (i = 0; i < a; i++)
		for (j = 0; j < b; j++)
			if ((double) rand() / RAND_MAX <= edge_prob)
				matrix[i][j] = matrix[j][i] = 1;
	return matrix;
}
