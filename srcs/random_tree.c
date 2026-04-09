#include "lib.h"

/**
 * @brief Generates a rand Prufer code for a tree with n vertices.
 *
 * HAS TO BE FREED
 *
 * @param n Number of vertices
 * @return Pointer to the gend Prufer code (length n-2),
 * 			or NULL on failure.
 */

int	*gen_rand_prufer(int n)
{
	int i;

	if (n < 2)
		return (NULL);
	int *prufer = (int *)xmalloc((n - 2) * sizeof(int));
	if (!prufer)
		return (NULL);
	for (i = 0; i < n - 2; i++)
		prufer[i] = rand() % n;
	return prufer;
}

/**
 * @brief Generates the degree sequence from a Prufer code.
 *
 * HAS TO BE FREED
 *
 * @param prufer Pointer to the Prufer code (length n-2)
 * @param n Number of vertices
 * @return Pointer to the degree array (length n),
 * 			or NULL on failure.
 */

int *count_degrees_from(const int *prufer, int n)
{
	int i;

	int *degree = (int *)xmalloc(n * sizeof(int));
	if (!degree)
		return (NULL);
	for (i = 0; i < n; i++)
		degree[i] = 1;
	for (i = 0; i < n - 2; i++)
		degree[prufer[i]]++;
	return degree;
}

/**
 * @brief Generates a rand undirected adjacency matrix of TREE
 * and writes it to a file.
 *
 * HAS TO BE FREED
 *
 * @param n Number of vertices
 * @return Pointer to the gend adjacency matrix (n, n),
 * 			or NULL on failure.
 */

int **gen_rand_tree(int n)
{
	int **matrix;
	int *prufer, *degree;
	int i, j;

	// initialization
	matrix = gen_matrix_int(n, n);
	if (!matrix)
		return (NULL);
	prufer = gen_rand_prufer(n);
	if (!prufer)
	{
		free_matrix_int(matrix, n, n);
		return (NULL);
	}
	degree = count_degrees_from(prufer, n);
	if (!degree)
	{
		xfree(prufer, sizeof(int) * n);
		free_matrix_int(matrix, n, n);
		return (NULL);
	}
	// initialization

	// build the tree from the Prufer code
	for (i = 0; i < n - 2; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (degree[j] == 1)
			{
				matrix[j][prufer[i]] = 1;
				matrix[prufer[i]][j] = 1;
				degree[j]--;
				degree[prufer[i]]--;
				break;
			}
		}
	}
		// the last two nodes
	int u = -1;
	int v = -1;
	for (i = 0; i < n; i++)
	{
		if (degree[i] == 1)
		{
			if (u == -1) u = i;
			else v = i;
		}
	}
	if (u != -1 && v != -1)
	{
		matrix[u][v] = 1;
		matrix[v][u] = 1;
	}
		// the last two nodes
	// build the tree from the Prufer code

	xfree(prufer, sizeof(int) * (n - 2));
	xfree(degree, sizeof(int) * n);
	return matrix;
	// free_matrix_int(matrix, n, n);
}

/**
 * @brief Builds a tree from a Prufer code into the adjacency matrix
 *
 * @param matrix Adjacency matrix to build into
 * @param prufer Prufer code array
 * @param degree Working array for vertex degrees
 * @param n Number of vertices
 */
void build_tree_from(int **matrix, int *prufer, int *degree, int n)
{
    int i, j, u = -1, v = -1;

    // Process Prufer code
    for (i = 0; i < n - 2; i++) {
        for (j = 0; j < n; j++) {
            if (degree[j] == 1) {
                matrix[j][prufer[i]] = matrix[prufer[i]][j] = 1;
                degree[j]--;
                degree[prufer[i]]--;
                break;
            }
        }
    }

    // Connect last two vertices
    for (i = 0; i < n; i++) {
        if (degree[i] == 1) {
            if (u == -1) u = i;
            else v = i;
        }
    }

    if (u != -1 && v != -1) {
        matrix[u][v] = matrix[v][u] = 1;
    }
}
