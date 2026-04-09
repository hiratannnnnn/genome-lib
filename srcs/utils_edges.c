#include "lib.h"

Edge	*create_edge(int id, int from, int to, double cost)
{
	Edge	*edge;

	edge = (Edge *)xmalloc(sizeof(Edge));
	if (!edge)
		return (NULL);
	edge->id = id;
	edge->from = from; // id of vertice;
	edge->to = to;
	edge->cost = cost;
	edge->next = NULL;
	return (edge);
}

int 	len_edge_list(Edge *head)
{
	int count;

	count = 0;
	while (head)
	{
		count++;
		head = head->next;
	}
	return count;
}

void	add_directed_edge(Vertex **vs, int from, int to, int edge_id, double cost)
{
	Edge *edge = create_edge(edge_id, from, to, cost);
	if (!edge)
		return;
	edge->next = vs[from]->incidence;
	vs[from]->incidence = edge;
}

void	add_undirected_edge(Vertex **vs, int from, int to, int edge_id, double cost)
{
	add_directed_edge(vs, from, to, edge_id, cost);
	add_directed_edge(vs, to, from, edge_id, cost);
}

Edge *get_last_edge(Edge *head)
{
	while (head && head->next)
		head = head->next;
	return (head);
}

Edge *get_target_edge(Edge *head, int t)
{
	while (head)
	{
		if (head->to == t)
			return head;
		head = head->next;
	}
	return (NULL);
}
