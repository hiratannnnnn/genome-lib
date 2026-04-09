#include "lib.h"

/**
 * @brief Returns the degree of each vertex based on the adjacency matrix
 *
 * @param matrix The adjacency matrix
 * @param n Number of vertices
 * @param degree Array to store degree values
 */
void compute_degrees(int **matrix, int n, int *degree)
{
    int i, j;

    for (i = 0; i < n; i++)
    {
        degree[i] = 0;
        for (j = 0; j < n; j++)
            if (matrix[i][j])
                degree[i]++;
    }
}

int     need_double_edge(int *degree, int n)
{
    int i;
    if (n % 2 == 1)
        return (0);
    for (i = 0; i < n; i++)
    {
        if (degree[i] == n - 1)
			return (1);
    }
	return (0);
}

int		count_edges(int *degree, int n)
{
	int sum, i;

	sum = 0;
	for (i = 0; i < n; i++)
		sum += degree[i];
	return sum / 2;
}

/**
 * @brief Returns the degree of each vertex based on the adjacency list.
 *
 * @param vs The adjacency List
 * @param n Number of vertices
 * @param degree Array to store degree values
 */

void compute_degrees_from_list(Vertex **vs, int n, int *degree)
{
	int i;
	Edge *list;

	for (i = 0; i < n; i++)
	{
		list = vs[i]->incidence;
		while (list)
		{
			degree[list->to]++;
			list = list->next;
		}
	}
}

static void	dfs(int **matrix, int n, int vertex, int *visited);

/**
 * @brief Determines if the given adjacency matrix represents a tree.
 *
 * A tree is defined as a connected,
	acyclic graph with n vertices and n-1 edges.

	* This function checks if the number of edges is n-1 and if the graph is connected
 * by performing a depth-first search (DFS) from vertex 0.
 *
 * @param matrix Pointer to the 2D adjacency matrix (size n x n)
 * @param n Number of vertices (size of the matrix)
 * @return 1 if the matrix represents a tree, 0 otherwise
 */

int	is_tree(int **matrix, int n)
{
	int i, j;
	int	edge_count;
	int	*visited;

	edge_count = 0;
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++)
			if (matrix[i][j])
				edge_count++;
	if (edge_count != n - 1)
		return (0);
	visited = (int *)xcalloc(n, sizeof(int));
	dfs(matrix, n, 0, visited);
	for (int i = 0; i < n; i++)
		if (!visited[i])
			return (xfree(visited, sizeof(int) * n), 0);
	xfree(visited, sizeof(int) * n);
	return (1);
}

/**
 * @brief Check if the @param matrix is CONNECTED or not.
 *
 * if all vertices are connected each other, each element of
 * @param visited will be 1.
 */
static void	dfs(int **matrix, int n, int vertex, int *visited)
{
	visited[vertex] = 1;
	for (int i = 0; i < n; i++)
		if (matrix[vertex][i] && !visited[i])
			dfs(matrix, n, i, visited);
}

/**
 * @brief Identifies vertices with odd degree and stores them in odd_list
 *
 * @param degree Array of vertex degrees
 * @param n Number of vertices
 * @param odd_list Array to store indices of odd-degree vertices
 * @return Number of odd-degree vertices found
 */
int find_odd_vertices(int *degree, int n, int *odd_list)
{
    int i, odd_count = 0;

    for (i = 0; i < n; i++)
        if (degree[i] % 2 == 1)
            odd_list[odd_count++] = i;

    return odd_count;
}

int is_undigraph(int **matrix, int n)
{
	int i, j;

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (matrix[i][j] != matrix[j][i])
				return (0);
		}
	}
	return (1);
}
