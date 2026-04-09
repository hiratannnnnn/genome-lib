#include "lib.h"

/**
 * @brief Writes an adjacency matrix to a text file.
 *
 * The file will contain the number of vertices n in the first line,
 * followed by n lines each containing n integers (0 or 1) separated by space.
 *
 * @param matrix	Pointer to the 2D adjacency matrix (n, n)
 * @param n			Number of vertices (matrix size)
 * @param filename	Output file name
 *
 * @return 0		on success
 * @return -1		if the file could not be opened
 * @return -2 		if matrix or matrix[i] is NULL
 *
 * @note
 * - The file is opened in "w" mode.
 * - Error handling and memory deallocation must be performed by the caller.
 */

int write_adjacent_matrix(int **matrix, int n, char const *filename)
{
	FILE 	*fp;
	int 	i, j;

	if (!matrix)
		return (-2);
	fp = fopen(filename, "w");
	if (!fp)
	{
		printf("Failed to open file.\n");
		return (-1);
	}
	fprintf(fp, "%d\n", n);
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			if (!matrix[i])
				return (fclose(fp), -2);
			fprintf(fp, "%d%c", matrix[i][j], (j == n - 1) ? '\n' : ' ');
		}
	fclose(fp);
	return (0);
}

int write_double_matrix(double **matrix, int r, int c, char const *filename)
{
    FILE *fp;
    int i, j;

    if (!matrix)
        return -2;
    fp = fopen(filename, "w");
    if (!fp)
    {
        printf("Failed to open file %s\n", filename);
        return -1;
    }
    fprintf(fp, "%d %d\n", r, c);
    for (i = 0; i < r; i++)
    {
        if (!matrix[i])
        {
            fclose(fp);
            return -2;
        }
        for (j = 0; j < c; j++)
            fprintf(fp, "%.10f%c", matrix[i][j], (j == c - 1) ? '\n' : ' ');
    }
    fclose(fp);
    return 0;
}

/**
 * @brief write adjacent list to file
 *
 * @param matrix adjacency matrix size of (n, n)
 * @param n 	 size
 * @param filename name of file
 */

int write_adjacent_list(int **matrix, int n, char const *filename)
{
	FILE 	*fp;
	int 	i, j, first;

	if (!matrix)
		return (-2);
	fp = fopen(filename, "w");
	if (!fp)
	{
		printf("Failed to open file.\n");
		return (-1);
	}
	fprintf(fp, "%d\n", n);
	for (i = 0; i < n; i++)
	{
		first = 1;
		fprintf(fp, "%d:", i);
		for (j = 0; j < n; j++)
		{
			if (matrix[i][j])
			{
				if (first)
					first = 0;
				else
					fprintf(fp, " ");
				fprintf(fp, "%d", j);
			}
		}
		fprintf(fp, "%c", '\n');
	}
	fclose(fp);
	return (0);
}

int 	write_path_node(Node *head, char const *filename)
{
	FILE *fp;
	Vertex *v;

	if (!head)
		return -1;
	fp = fopen(filename, "w");
	if (!fp)
	{
		printf("failed to open file %s\n", filename);
		return (-2);
	}
	while (head)
	{
		v = (Vertex *)head->ptr;
		fprintf(fp, "%d%c", v->id, head->next ? ' ' : '\n');
		head = head->next;
	}
	fclose(fp);
	return (0);
}

int write_perm(int *perm, int n, char const *filename)
{
    FILE *fp;
    int i;

    if (!perm || n <= 0)
        return (-2);
    fp = fopen(filename, "w");
    if (!fp)
    {
        printf("Failed to open file %s\n", filename);
        return (-1);
    }
    fprintf(fp, "%d\n", n);
    for (i = 0; i < n; i++)
        {fprintf(fp, "%d%c", perm[i], (i == n - 1) ? '\n' : ' ');}
	for (i = 0; i < n; i++)
		fprintf(fp, "%d%c", i+1, (i == n - 1) ? '\n' : ' ');
    fclose(fp);
    return (0);
}

void 	save_some_matrix(int n, double edge_prob,
	int **(*gen_some_mat)(int, double), char const *filename)
{
	int **matrix;

	matrix = gen_some_mat(n, edge_prob);
	if (!matrix)
		return ;
	write_adjacent_matrix(matrix, n, filename);
	free_matrix_int(matrix, n, n);
}
