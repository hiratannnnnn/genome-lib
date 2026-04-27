#include "lib.h"
#include "test_framework.h"

/* ---- helpers ---- */

static int	edge_list_is_sorted_asc(Edge *head)
{
	while (head && head->next)
	{
		if (head->to > head->next->to)
			return (0);
		head = head->next;
	}
	return (1);
}

static int	edge_list_is_sorted_desc(Edge *head)
{
	while (head && head->next)
	{
		if (head->to < head->next->to)
			return (0);
		head = head->next;
	}
	return (1);
}

static int	node_vertex_is_sorted_asc(Node *head)
{
	while (head && head->next)
	{
		if (((Vertex *)head->ptr)->id > ((Vertex *)head->next->ptr)->id)
			return (0);
		head = head->next;
	}
	return (1);
}

static int	node_edge_cost_is_sorted_asc(Node *head)
{
	while (head && head->next)
	{
		if (((Edge *)head->ptr)->cost > ((Edge *)head->next->ptr)->cost
			+ DBL_EPSILON)
			return (0);
		head = head->next;
	}
	return (1);
}

/* ---- tests ---- */

void	run_tests_sort(void)
{
	Edge	*e1;
	Edge	*e2;
	Edge	*e3;
	Vertex	**vs;
	Node	*head;
		int degree[] = {1, 3, 2};
	Edge	*e;
	int		ok;

	TEST_GROUP("cmp_int_asc / cmp_int_desc");
	{
		/* cmp_int_asc(a,b) = a - b: positive -> swap for descending, negative
			-> keep for asc */
		ASSERT(cmp_int_asc(1, 2) < 0, "cmp_int_asc(1,2) < 0");
		ASSERT(cmp_int_asc(2, 1) > 0, "cmp_int_asc(2,1) > 0");
		ASSERT(cmp_int_asc(3, 3) == 0, "cmp_int_asc(3,3) == 0");
		/* cmp_int_desc(a,b) = b - a */
		ASSERT(cmp_int_desc(1, 2) > 0, "cmp_int_desc(1,2) > 0");
		ASSERT(cmp_int_desc(2, 1) < 0, "cmp_int_desc(2,1) < 0");
		ASSERT(cmp_int_desc(5, 5) == 0, "cmp_int_desc(5,5) == 0");
	}
	TEST_GROUP("cmp_double_asc / cmp_double_desc");
	{
		/* cmp_double_asc returns 1 if a > b, 0 otherwise */
		ASSERT(cmp_double_asc(2.0, 1.0) == 1, "cmp_double_asc(2,1) = 1");
		ASSERT(cmp_double_asc(1.0, 2.0) == 0, "cmp_double_asc(1,2) = 0");
		ASSERT(cmp_double_asc(1.0, 1.0) == 0, "cmp_double_asc(1,1) = 0");
		/* cmp_double_desc returns 1 if b > a, 0 otherwise */
		ASSERT(cmp_double_desc(1.0, 2.0) == 1, "cmp_double_desc(1,2) = 1");
		ASSERT(cmp_double_desc(2.0, 1.0) == 0, "cmp_double_desc(2,1) = 0");
	}
	TEST_GROUP("cmp_edge_cost");
	{
		e1 = create_edge(0, 0, 1, 1.0);
		e2 = create_edge(1, 1, 2, 2.0);
		e3 = create_edge(2, 2, 3, 1.0);
		ASSERT(cmp_edge_cost(e1, e2) < 0, "cmp_edge_cost: cost 1.0 < 2.0");
		ASSERT(cmp_edge_cost(e2, e1) > 0, "cmp_edge_cost: cost 2.0 > 1.0");
		ASSERT(cmp_edge_cost(e1, e3) == 0, "cmp_edge_cost: equal costs -> 0");
		xfree(e1, sizeof(Edge));
		xfree(e2, sizeof(Edge));
		xfree(e3, sizeof(Edge));
	}
	TEST_GROUP("sort_list (sort Edge list by .to)");
	{
		/* Build unsorted edge list: to=3, to=1, to=2 */
		vs = create_vertex_array(4);
		add_directed_edge(vs, 0, 3, 0, 0.0);
		add_directed_edge(vs, 0, 1, 1, 0.0);
		add_directed_edge(vs, 0, 2, 2, 0.0);
		sort_list(&vs[0]->incidence, cmp_int_asc);
		ASSERT(edge_list_is_sorted_asc(vs[0]->incidence),
			"sort_list(cmp_int_asc): edges sorted by .to ascending");
		sort_list(&vs[0]->incidence, cmp_int_desc);
		ASSERT(edge_list_is_sorted_desc(vs[0]->incidence),
			"sort_list(cmp_int_desc): edges sorted by .to descending");
		free_vertex_array(vs, 4);
	}
	TEST_GROUP("sort_nodes_vertex_id");
	{
		vs = create_vertex_array(4);
		head = NULL;
		/* Add in order: id=3, id=1, id=0, id=2 */
		append_node(&head, create_node(vs[3], NODE_TYPE_VERTEX));
		append_node(&head, create_node(vs[1], NODE_TYPE_VERTEX));
		append_node(&head, create_node(vs[0], NODE_TYPE_VERTEX));
		append_node(&head, create_node(vs[2], NODE_TYPE_VERTEX));
		sort_nodes_vertex_id(&head, cmp_int_asc);
		ASSERT(node_vertex_is_sorted_asc(head),
			"sort_nodes_vertex_id(cmp_int_asc): sorted by vertex id ascending");
		ASSERT(((Vertex *)head->ptr)->id == 0,
			"sort_nodes_vertex_id: first node is id=0");
		free_nodes(&head);
		free_vertex_array(vs, 4);
	}
	TEST_GROUP("sort_nodes_edge_cost");
	{
		e1 = create_edge(0, 0, 1, 3.0);
		e2 = create_edge(1, 1, 2, 1.0);
		e3 = create_edge(2, 2, 3, 2.0);
		head = NULL;
		append_node(&head, create_node(e1, NODE_TYPE_EDGE));
		append_node(&head, create_node(e2, NODE_TYPE_EDGE));
		append_node(&head, create_node(e3, NODE_TYPE_EDGE));
		sort_nodes_edge_cost(&head, cmp_double_asc);
		ASSERT(node_edge_cost_is_sorted_asc(head),
			"sort_nodes_edge_cost(asc): sorted by cost ascending");
		ASSERT(((Edge *)head->ptr)->cost == 1.0,
			"sort_nodes_edge_cost: first edge has cost 1.0");
		free_nodes(&head);
		xfree(e1, sizeof(Edge));
		xfree(e2, sizeof(Edge));
		xfree(e3, sizeof(Edge));
	}
	TEST_GROUP("sort_list_by_degree");
	{
		/*
			* sort_list_by_degree: sorts edge list by degree of edge->to vertex.
			* degree[0]=1, degree[1]=3, degree[2]=2
			* Edges: to=1, to=0, to=2
			* Sorted asc by degree: to=0 (deg=1), to=2 (deg=2), to=1 (deg=3)
			*/
		vs = create_vertex_array(3);
		add_directed_edge(vs, 0, 1, 0, 0.0); /* added last -> prepended first */
		add_directed_edge(vs, 0, 0, 1, 0.0);
		add_directed_edge(vs, 0, 2, 2, 0.0);
		/* list order (prepend): to=2, to=0, to=1 */
		sort_list_by_degree(&vs[0]->incidence, degree, cmp_int_asc);
		e = vs[0]->incidence;
		ok = (e && e->to == 0 && e->next && e->next->to == 2 && e->next->next
				&& e->next->next->to == 1);
		ASSERT(ok,
			"sort_list_by_degree(asc): sorted by degree of target vertex");
		free_vertex_array(vs, 3);
	}
}
