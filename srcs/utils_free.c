#include "lib.h"

void	free_array_int(int *ptr, int const n)
{
	if (!ptr)
		return ;
	xfree(ptr, sizeof(int) * n);
}

void	free_matrix_int(int **matrix, int const r, int const c)
{
	int i;

	if (!matrix)
		return ;
	for (i = 0;  i < r; i++)
		if (matrix[i])
			free_array_int(matrix[i], c);
	xfree(matrix, sizeof(int *) * r);
}

void	free_array_double(double *ptr, int const c)
{
	if (!ptr)
		return ;
	xfree(ptr, sizeof(double) * c);
}

void	free_matrix_double(double **matrix, int const r, int const c)
{
	int i;

	if (!matrix)
		return ;
	for (i = 0; i < r; i++)
		free_array_double(matrix[i], c);
	xfree(matrix, sizeof(double *) * r);
}

void	free_edge_list(Edge *head)
{
	Edge	*cur;
	Edge	*next;

	cur = head;
	while (cur)
	{
		next = cur->next;
		xfree(cur, sizeof(Edge));
		cur = next;
	}
}

void	free_vertex(Vertex *vertex)
{
	if (!vertex)
		return ;
	free_edge_list(vertex->incidence);
	xfree(vertex, sizeof(Vertex));
}

void	free_vertex_array(Vertex **vs, int n)
{
	int	i;

	if (!vs)
		return ;
	i = 0;
	while (i < n)
	{
		free_vertex(vs[i]);
		i++;
	}
	xfree(vs, sizeof(Vertex *) * n);
}
