#include "lib.h"

/**
 * @brief Generates a rand directed adjacency matrix and writes it to a file.
 *
 * HAS TO BE FREED
 *
 * @param n Number of vertices
 * @param edge_prob 0.0～1.0, edge probability between any two vertices
 */

int 	**gen_rand_digraph(int n, double edge_prob)
{
	int **matrix;
	int i, j;

	matrix = gen_matrix_int(n, n);
	if (!matrix)
		return (NULL);
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			if (i == j) continue;
			if ((double)rand() / (RAND_MAX + 1.0) < edge_prob)
				matrix[i][j] = 1;
		}
	return (matrix);
}

int 	**gen_rand_DAG(int n, double edge_prob)
{
	int **matrix;
	int i, j;
	int *topnr;

	matrix = gen_matrix_int(n, n);
	if (!matrix)
		return (NULL);
	topnr = rand_perm(n);
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			if (topnr[i] < topnr[j])
				if (topnr[i] + 1 == topnr[j] ||
					((double)rand() / (RAND_MAX + 1.0) < edge_prob))
					matrix[i][j] = 1;
	free_array_int(topnr, n);
	return (matrix);
}
