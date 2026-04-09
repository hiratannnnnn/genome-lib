#include "lib.h"

/**
 * @brief Generates a rand undirected adjacency matrix and writes it to a file.
 *
 * @param n Number of vertices
 * @param edge_prob 0.0～1.0, edge probability between any two vertices
 */

int 	**gen_rand_undigraph(int n, double edge_prob)
{
	int **matrix;
	int i, j;

	matrix = gen_matrix_int(n, n);
	if (!matrix)
		return (NULL);
	for (i = 0; i < n; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			if ((double)rand() / RAND_MAX < edge_prob)
			{
				matrix[i][j] = 1;
				matrix[j][i] = 1;
			}
		}
	}
	return (matrix);
}

int 	**gen_rand_conn_undigraph(int n, double edge_prob)
{
	int **matrix;
	int i, j;

	matrix = gen_rand_tree(n);
	if (!matrix)
		return NULL;

	if (edge_prob <= 0.0)
		return matrix;

	for (i = 0; i < n; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			if (!matrix[i][j] && (double) rand() / RAND_MAX <= edge_prob)
			{
				matrix[i][j] = 1;
				matrix[j][i] = 1;
			}
		}
	}
	return matrix;
}
