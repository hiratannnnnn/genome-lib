#include "lib.h"

double **gen_matrix_double(int r, int c)
{
	int i;
	double **matrix;

	matrix = (double **)xmalloc(sizeof(double *) * r);
	if (!matrix)
		return NULL;
	for (i = 0; i < r; i++)
	{
		matrix[i] = (double *)xcalloc(c, sizeof(double));
		if (!matrix[i])
		{
			free_matrix_double(matrix, i, c);
			return NULL;
		}
	}
	return matrix;
}
