#include "lib.h"

double **gen_rand_digraph_dbcost(int n, double min, double max, double edge_prob)
{
	double **matrix;
	int i, j;
    double size;

    size = max - min;
	matrix = gen_matrix_double(n, n);
	if (!matrix)
		return NULL;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
        {
            if (i == j)
                continue;
            if ((double) rand() / RAND_MAX <= edge_prob)
                matrix[i][j] = (double)rand() / RAND_MAX * size + min;
            else 
                matrix[i][j] = DBL_MAX / 2;
        }
   	return matrix;
}

int **gen_rand_digraph_intcost(int n, int min, int max, double edge_prob)
{
    int **matrix;
    int i, j;
    unsigned int size;

    size = max - min + 1;
    matrix = gen_matrix_int(n, n);
    if (!matrix)
        return NULL;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
        {
            if (i == j)
                continue;
            if ((double)rand() / RAND_MAX <= edge_prob)
                matrix[i][j] = rand() % size + min; // 1 ~ max
            else
                matrix[i][j] = INT_MAX / 2;
        }
    return matrix;
}
