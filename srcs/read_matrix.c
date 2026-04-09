#include "lib.h"

/**
 * @brief Reads an adjacency matrix from a file.
 *
 * The file should have the number of vertices n in the first line,
 * followed by n lines each containig n integers (0 or 1) separated by spaces.
 *
 * the length of lines and cols of the file must be the same.
 *
 * @param *n pointer to the size n
 * @param filename The name of the file to read from
 * @return Pointer to the allocated 2D adjacency matrix, or NULL on failure.
 */

int **read_adj(int *n, char const *filename)
{
	FILE *fp;
	int **matrix;
	int i, j;

	// opening file
	fp = fopen(filename, "r");
	if (!fp)
		return (NULL);
	// opening file

	// set size n
	if (fscanf(fp, "%d", n) != 1)
	{
		fclose(fp);
		return (NULL);
	}
	// set size n
	// initialize matrix
	matrix = gen_matrix_int(*n, *n);
	if (!matrix)
	{
		fclose(fp);
		return (NULL);
	}
	// initialize matrix *** has to be freed
	// set matrix
	for (i = 0; i < *n; i++)
	{
		for (j = 0; j < *n; j++)
		{
			if (fscanf(fp, "%d", &matrix[i][j]) != 1)
			{
				free_matrix_int(matrix, i, *n);
				fclose(fp);
				return (NULL);
			}
		}
	}
	// set matrix

	fclose(fp);
	return matrix;
}	// *** has to be freed


/**
 * @brief Reads an adjacency matrix from a file.
 *
 * The file should have the number of vertices n in the first line,
 * followed by n lines each containig n integers (0 or 1) separated by spaces.
 *
 * the length of lines and cols of the file must be the same.
 *
 * @param *n pointer to the size n
 * @param filename The name of the file to read from
 * @return Pointer to the allocated 2D adjacency matrix, or NULL on failure.
 */

int **read_list(int *n, char const *filename)
{
	FILE *fp;
	int **matrix;
	int i;
	int vertex, neighbor;
	char line[1024], *p;

	// opening file
	fp = fopen(filename, "r");
	if (!fp)
		return (NULL);
	// opening file

	// set size n
	if (fscanf(fp, "%d", n) != 1)
	{
		fclose(fp);
		return (NULL);
	}
	// set size n

	// initialize matrix
	matrix = gen_matrix_int(*n, *n);
	if (!matrix)
	{
		fclose(fp);
		return (NULL);
	}
	// initialize matrix *** has to be freed

	// set matrix
	i = 0;
	while (i < *n && fgets(line, sizeof(line), fp))
	{
		p = line;
		if (sscanf(p, "%d:", &vertex) != 1)
			continue;
		p = strchr(p, ':');
		if (p) p++;
		while (p && sscanf(p, "%d", &neighbor) == 1)
		{
			matrix[vertex][neighbor] = 1;
			while (*p && *p != ' ' && *p != '\n') p++;
			while (*p == ' ') p++;
		}
		i++;
	}
	// set matrix

	fclose(fp);
	return matrix;
}	// *** has to be freed


int **read_matrix(int *a, int *b, char const *filename)
{
	FILE *fp;
	int **matrix;
	int i, j;

	fp = fopen(filename, "r");
	if (!fp)
		return (NULL);
	if (fscanf(fp, "%d %d", a, b) != 2)
	{
		fclose(fp);
		return (NULL);
	}
	matrix = gen_matrix_int(*a, *b);
	if (!matrix)
	{
		fclose(fp);
		return (NULL);
	}
	for (i = 0; i < *a; i++)
	{
		for (j = 0; j < *b; j++)
		{
			if (fscanf(fp, "%d", &matrix[i][j]) != 1)
			{
				free_matrix_int(matrix, i, *b);
				fclose(fp);
				return (NULL);
			}
		}
	}
	// set matrix

	fclose(fp);
	return matrix;
}


double **read_double_matrix(int *r, int *c, char const *filename)
{
    FILE *fp;
    double **matrix;
    int i, j;

    fp = fopen(filename, "r");
    if (!fp)
        return NULL;

    // read size
    if (fscanf(fp, "%d %d", r, c) != 2)
    {
        fclose(fp);
        return NULL;
    }

    matrix = gen_matrix_double(*r, *c);
    if (!matrix)
    {
        fclose(fp);
        return NULL;
    }

    for (i = 0; i < *r; i++)
    {
        for (j = 0; j < *c; j++)
        {
            if (fscanf(fp, "%lf", &matrix[i][j]) != 1)
            {
                free_matrix_double(matrix, *r, *c);
                fclose(fp);
                return NULL;
            }
        }
    }

    fclose(fp);
    return matrix;
}
