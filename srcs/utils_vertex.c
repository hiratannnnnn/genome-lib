#include "lib.h"


Vertex	*create_vertex(int id)
{
	Vertex	*vertex;

	vertex = (Vertex *)xmalloc(sizeof(Vertex));
	if (!vertex)
		return (NULL);
	vertex->id = id;
	vertex->label = id;
	vertex->incidence = NULL;
	return (vertex);
}

Vertex	**create_vertex_array(int n)
{
	int		i;
	Vertex	**vs;

	vs = (Vertex **)xmalloc(sizeof(Vertex *) * n);
	if (!vs)
		return (NULL);
	i = 0;
	while (i < n)
	{
		vs[i] = create_vertex(i);
		if (!vs[i])
		{
			free_vertex_array(vs, i);
			return (NULL);
		}
		i++;
	}
	return (vs);
}

void reset_labels(Vertex **vs, int n)
{
	int i;

	for (i = 0; i < n; i++)
		vs[i]->label = i;
}

void	update_labels(PathNode *node, int label)
{
	while (node)
	{
		node->v->label = label;
		node = node->next;
	}
}

/**
 * @param asc for asc, 1. for desc, 0.
 */
void sort_list_cyclic(Vertex **vs, int n, int asc)
{
	Edge *list, *last, *target, *t_prev;
	int i, t, t_p;

	if (asc != 0 && asc != 1)
		return ;
	for (i = 0; i < n; i++)
	{
		list = vs[i]->incidence;
		if (asc)
		{
			sort_list(&list, cmp_int_asc);
			t = (i + 1) % n;
		}
		else
		{
			sort_list(&list, cmp_int_desc);
			t = (i + n - 1) % n;
		}
		t_p = (i + n - 2 + asc) % n;
		t = (t_p + 1 + asc) % n;

		// swap list (first) and target
		last = get_last_edge(list);
		if (!last)
			continue;
		target = get_target_edge(list, t);
		t_prev = get_target_edge(list, t_p);
		last->next = list;
		t_prev->next = NULL;
		vs[i]->incidence = target;
	}
}
