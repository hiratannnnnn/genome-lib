#include "lib.h"

void	identity_matrix(int **matrix, int n)
{
	int i, j;

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			matrix[i][j] = (i == j) ? 1 : 0;
}

void	permutation_matrix(int **matrix, int n, int i, int j)
{
	int k, tmp;

	if (i < 0 || j < 0 || i >= n || j >= n)
		return ;
	identity_matrix(matrix, n);
	for (k = 0; k < n; k++)
	{
		tmp = matrix[i][k];
		matrix[i][k] = matrix[j][k];
		matrix[j][k] = tmp;
	}
}

void	matrix_multiply(int **A, int **B, int **C, int n)
{
	int i, j, k;

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			C[i][j] = 0;
			for (k = 0; k < n; k++)
				C[i][j] += A[i][k] * B[k][j];

		}
	}
}

void	swap_vertex(int **matrix, int n, int i, int j)
{
	int k, tmp;

	for (k = 0; k < n; k++)
	{
		tmp = matrix[i][k];
		matrix[i][k] = matrix[j][k];
		matrix[j][k] = tmp;
	}
	for (k = 0; k < n; k++)
	{
		tmp = matrix[k][i];
		matrix[k][i] = matrix[k][j];
		matrix[k][j] = tmp;
	}
}

void	shuffle_vertices(int **matrix, int n)
{
	int i, j;

	for (i = 0; i < n - 1; i++)
	{
		j = i + rand() % (n - i);
		swap_vertex(matrix, n, i, j);
	}
}
