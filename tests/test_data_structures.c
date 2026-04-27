#include "lib.h"
#include "test_framework.h"

/* ===== Edge ===== */

static void	test_edges(void)
{
	Edge	*e;
	Edge	*e2;
	Vertex	**vs;

	TEST_GROUP("Edge: create / len / get_last / get_target");
	{
		e = create_edge(0, 1, 2, 3.14);
		ASSERT(e != NULL, "create_edge: non-NULL");
		ASSERT(e->id == 0, "create_edge: id");
		ASSERT(e->from == 1, "create_edge: from");
		ASSERT(e->to == 2, "create_edge: to");
		ASSERT(e->cost == 3.14, "create_edge: cost");
		ASSERT(e->next == NULL, "create_edge: next is NULL");
		e2 = create_edge(1, 2, 3, 1.0);
		e->next = e2;
		ASSERT(len_edge_list(e) == 2, "len_edge_list: 2 edges");
		ASSERT(len_edge_list(e2) == 1, "len_edge_list: 1 edge from middle");
		ASSERT(len_edge_list(NULL) == 0, "len_edge_list: NULL = 0");
		ASSERT(get_last_edge(e) == e2, "get_last_edge: returns tail");
		ASSERT(get_last_edge(NULL) == NULL, "get_last_edge(NULL): NULL");
		ASSERT(get_target_edge(e, 2) == e,
			"get_target_edge: finds first edge (to=2)");
		ASSERT(get_target_edge(e, 3) == e2,
			"get_target_edge: finds second edge (to=3)");
		ASSERT(get_target_edge(e, 99) == NULL,
			"get_target_edge: missing target = NULL");
		xfree(e, sizeof(Edge));
		xfree(e2, sizeof(Edge));
	}
	TEST_GROUP("Edge: add_directed_edge / add_undirected_edge");
	{
		vs = create_vertex_array(3);
		add_directed_edge(vs, 0, 1, 0, 1.0);
		add_directed_edge(vs, 0, 2, 1, 2.0);
		ASSERT(len_edge_list(vs[0]->incidence) == 2,
			"add_directed_edge: 2 edges from vertex 0");
		ASSERT(vs[1]->incidence == NULL,
			"add_directed_edge: vertex 1 has no outgoing edges");
		ASSERT(vs[2]->incidence == NULL,
			"add_directed_edge: vertex 2 has no outgoing edges");
		add_undirected_edge(vs, 1, 2, 2, 5.0);
		ASSERT(len_edge_list(vs[1]->incidence) == 1,
			"add_undirected_edge: vertex 1 gets 1 edge");
		ASSERT(len_edge_list(vs[2]->incidence) == 1,
			"add_undirected_edge: vertex 2 gets 1 edge");
		ASSERT(vs[1]->incidence->to == 2,
			"add_undirected_edge: v1->v2 direction correct");
		ASSERT(vs[2]->incidence->to == 1,
			"add_undirected_edge: v2->v1 direction correct");
		free_vertex_array(vs, 3);
	}
}

/* ===== Vertex ===== */

static void	test_vertices(void)
{
	Vertex	*v;
	Vertex	**vs;
	Node	*n0;
	Node	*n1;
	Node	*n2;
	Node	*head;

	TEST_GROUP("Vertex: create / create_array / reset_labels");
	{
		v = create_vertex(5);
		ASSERT(v != NULL, "create_vertex: non-NULL");
		ASSERT(v->id == 5, "create_vertex: id set");
		ASSERT(v->label == 5, "create_vertex: label initialized to id");
		ASSERT(v->incidence == NULL, "create_vertex: incidence is NULL");
		xfree(v, sizeof(Vertex));
		vs = create_vertex_array(4);
		ASSERT(vs != NULL, "create_vertex_array: non-NULL");
		ASSERT(vs[0]->id == 0, "create_vertex_array: vs[0].id = 0");
		ASSERT(vs[3]->id == 3, "create_vertex_array: vs[3].id = 3");
		vs[0]->label = 99;
		vs[1]->label = 88;
		reset_labels(vs, 4);
		ASSERT(vs[0]->label == 0, "reset_labels: vs[0].label reset to id (0)");
		ASSERT(vs[1]->label == 1, "reset_labels: vs[1].label reset to id (1)");
		free_vertex_array(vs, 4);
	}
	TEST_GROUP("Vertex: update_labels via Node list");
	{
		vs = create_vertex_array(3);
		n0 = create_node(vs[0], NODE_TYPE_VERTEX);
		n1 = create_node(vs[1], NODE_TYPE_VERTEX);
		n2 = create_node(vs[2], NODE_TYPE_VERTEX);
		head = NULL;
		append_node(&head, n0);
		append_node(&head, n1);
		append_node(&head, n2);
		update_labels(head, 42);
		ASSERT(vs[0]->label == 42, "update_labels: vs[0] updated");
		ASSERT(vs[1]->label == 42, "update_labels: vs[1] updated");
		ASSERT(vs[2]->label == 42, "update_labels: vs[2] updated");
		free_nodes(&head);
		free_vertex_array(vs, 3);
	}
}

/* ===== Node ===== */

static void	test_nodes(void)
{
	Edge	*e;
	Vertex	*v;
	Node	*ne;
	Node	*nv;
	Node	*queue;
	Node	*popped;
	Node	*q2;
	Edge	*ea;
	Edge	*eb;
	Node	*na;
	Node	*nb;
	Node	*dq;
	Edge	*e1;
	Edge	*e2;
	Edge	*e3;
	Node	*n1;
	Node	*n2;
	Node	*n3;
	Node	*head;
	double	total;
	Edge	*e4;
	Node	*n4;
	Node	*head2;

	TEST_GROUP("Node: create / append / count / pop_first / enqueue / dequeue / type access");
	{
		e = create_edge(0, 0, 1, 1.0);
		v = create_vertex(7);
		ne = create_node(e, NODE_TYPE_EDGE);
		ASSERT(ne != NULL, "create_node(edge): non-NULL");
		ASSERT(ne->type == NODE_TYPE_EDGE, "create_node(edge): type set");
		ASSERT(node_get_edge(ne) == e, "node_get_edge: returns correct edge");
		ASSERT(node_get_vertex(ne) == NULL,
			"node_get_vertex on edge node: NULL");
		nv = create_node(v, NODE_TYPE_VERTEX);
		ASSERT(nv->type == NODE_TYPE_VERTEX, "create_node(vertex): type set");
		ASSERT(node_get_vertex(nv) == v,
			"node_get_vertex: returns correct vertex");
		ASSERT(node_get_edge(nv) == NULL, "node_get_edge on vertex node: NULL");
		queue = NULL;
		append_node(&queue, ne);
		append_node(&queue, nv);
		ASSERT(count_nodes(queue) == 2, "count_nodes: 2 nodes");
		ASSERT(count_nodes(NULL) == 0, "count_nodes(NULL): 0");
		popped = pop_first_node(&queue);
		ASSERT(popped == ne, "pop_first_node: returns first node");
		ASSERT(count_nodes(queue) == 1,
			"pop_first_node: length decreases to 1");
		free_node(popped);
		q2 = NULL;
		ea = create_edge(1, 1, 2, 2.0);
		eb = create_edge(2, 2, 3, 3.0);
		na = create_node(ea, NODE_TYPE_EDGE);
		nb = create_node(eb, NODE_TYPE_EDGE);
		enqueue_node(&q2, na);
		enqueue_node(&q2, nb);
		dq = dequeue_node(&q2);
		ASSERT(dq == na, "dequeue_node: FIFO - first in, first out");
		free_node(dq);
		free_nodes(&queue);
		ASSERT(queue == NULL, "free_nodes: queue set to NULL");
		free_nodes(&q2);
		xfree(e, sizeof(Edge));
		xfree(v, sizeof(Vertex));
		xfree(ea, sizeof(Edge));
		xfree(eb, sizeof(Edge));
	}
	TEST_GROUP("Node: insert_after / pop_last / merge / cost_of_nodes");
	{
		e1 = create_edge(0, 0, 1, 1.0);
		e2 = create_edge(1, 1, 2, 2.0);
		e3 = create_edge(2, 2, 3, 3.0);
		n1 = create_node(e1, NODE_TYPE_EDGE);
		n2 = create_node(e2, NODE_TYPE_EDGE);
		n3 = create_node(e3, NODE_TYPE_EDGE);
		head = NULL;
		append_node(&head, n1);
		append_node(&head, n3);
		/* insert n2 after n1 */
		insert_node_after(n1, n2);
		ASSERT(count_nodes(head) == 3, "insert_node_after: count becomes 3");
		ASSERT(n1->next == n2, "insert_node_after: n1->next is n2");
		ASSERT(n2->next == n3, "insert_node_after: n2->next is n3");
		/* cost_of_nodes */
		total = cost_of_nodes(head);
		ASSERT(total == 6.0, "cost_of_nodes: 1+2+3 = 6.0");
		/* pop_last */
		pop_last_node(&head);
		ASSERT(count_nodes(head) == 2, "pop_last_node: count becomes 2");
		ASSERT(n1->next == n2, "pop_last_node: n1->next still n2");
		/* merge */
		e4 = create_edge(3, 3, 4, 4.0);
		n4 = create_node(e4, NODE_TYPE_EDGE);
		head2 = NULL;
		append_node(&head2, n4);
		merge_nodes(&head, &head2);
		ASSERT(count_nodes(head) == 3, "merge_nodes: total 3 nodes");
		ASSERT(head2 == NULL, "merge_nodes: source becomes NULL");
		free_nodes(&head);
		xfree(e1, sizeof(Edge));
		xfree(e2, sizeof(Edge));
		xfree(e3, sizeof(Edge));
		xfree(e4, sizeof(Edge));
	}
}

void	run_tests_data_structures(void)
{
	test_edges();
	test_vertices();
	test_nodes();
}
