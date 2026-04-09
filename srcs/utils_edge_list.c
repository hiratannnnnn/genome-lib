#include "lib.h"

/**
 * @brief make edge list using Node struct from Adjacency List
 *
 * @param vs Vertex array
 * @param n
 * @param is_undir is undirected graph(1) or not(0)
 */
EdgeNode	*edge_list_from_adj_list(Vertex **vs, int n, int is_undir)
{
	Edge *edge;
	EdgeNode *edge_list;
	EdgeNode *new_node;
	Vertex *v;
	int i;

	edge_list = NULL;

	for (i = 0; i < n; i++)
	{
		v = vs[i];
		edge = v->incidence;
		while (edge)
		{
			if (!is_undir || (edge->from < edge->to))
			{
				new_node = create_edgenode(edge);
				append_edgenode(&edge_list, new_node);
			}
			edge = edge->next;
		}
	}
	return (edge_list);
}
