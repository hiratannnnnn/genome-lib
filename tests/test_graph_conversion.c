#include "lib.h"
#include "test_framework.h"

void	run_tests_graph_conversion(void)
{
	int		**m;
	Vertex	**vs;
	int		**m;
	Vertex	**vs;
	Vertex	**vs;
	int		**m;
	int		**orig;
	Vertex	**vs;
	int		**recovered;
	int		ok;
	int		**empty;
	int		**comp_empty;
	int		**k3;
	int		**comp_k3;
	int		**m;
	Vertex	**comp;
	double	**cm;
	Vertex	**vs;
	Edge	*e01;
	int		**cost;
	int		**adj;
	Vertex	**vs;
	Node	*el;
	Vertex	**vs2;
	Node	*el2;

	TEST_GROUP("adj_matrix_to_vertices (undirected)");
	{
		/*
			* Undirected path 0-1-2:
			* matrix[0][1]=matrix[1][0]=1, matrix[1][2]=matrix[2][1]=1
			* Undirected mode: upper triangle only -> 2 undirected edges added
			* => each vertex gets edges in both directions
			*/
		m = gen_matrix_int(3, 3);
		m[0][1] = m[1][0] = 1;
		m[1][2] = m[2][1] = 1;
		vs = adj_matrix_to_vertices(m, 3, 1);
		ASSERT(vs != NULL, "adj_matrix_to_vertices (undir): non-NULL");
		ASSERT(len_edge_list(vs[0]->incidence) == 1,
			"adj_matrix_to_vertices (undir): vs[0] has 1 edge");
		ASSERT(len_edge_list(vs[1]->incidence) == 2,
			"adj_matrix_to_vertices (undir): vs[1] has 2 edges");
		ASSERT(len_edge_list(vs[2]->incidence) == 1,
			"adj_matrix_to_vertices (undir): vs[2] has 1 edge");
		free_vertex_array(vs, 3);
		free_matrix_int(m, 3, 3);
	}
	TEST_GROUP("adj_matrix_to_vertices (directed)");
	{
		/* Directed: 0->1 and 1->2 only */
		m = gen_matrix_int(3, 3);
		m[0][1] = 1;
		m[1][2] = 1;
		vs = adj_matrix_to_vertices(m, 3, 0);
		ASSERT(vs != NULL, "adj_matrix_to_vertices (dir): non-NULL");
		ASSERT(len_edge_list(vs[0]->incidence) == 1,
			"adj_matrix_to_vertices (dir): vs[0] has 1 edge");
		ASSERT(len_edge_list(vs[1]->incidence) == 1,
			"adj_matrix_to_vertices (dir): vs[1] has 1 edge");
		ASSERT(len_edge_list(vs[2]->incidence) == 0,
			"adj_matrix_to_vertices (dir): vs[2] has 0 edges");
		free_vertex_array(vs, 3);
		free_matrix_int(m, 3, 3);
	}
	TEST_GROUP("vertices_to_adj_matrix");
	{
		/* Undirected path 0-1-2: vertices_to_adj_matrix should yield symmetric matrix */
		vs = create_vertex_array(3);
		add_undirected_edge(vs, 0, 1, 0, 1.0);
		add_undirected_edge(vs, 1, 2, 1, 1.0);
		m = vertices_to_adj_matrix(vs, 3);
		ASSERT(m != NULL, "vertices_to_adj_matrix: non-NULL");
		ASSERT(m[0][1] == 1 && m[1][0] == 1,
			"vertices_to_adj_matrix: edge 0-1");
		ASSERT(m[1][2] == 1 && m[2][1] == 1,
			"vertices_to_adj_matrix: edge 1-2");
		ASSERT(m[0][2] == 0 && m[2][0] == 0,
			"vertices_to_adj_matrix: no edge 0-2");
		ASSERT(m[0][0] == 0 && m[1][1] == 0,
			"vertices_to_adj_matrix: no self-loops");
		free_matrix_int(m, 3, 3);
		free_vertex_array(vs, 3);
	}
	TEST_GROUP("vertices_to_adj_matrix roundtrip");
	{
		/* matrix -> vertices -> matrix should reproduce the original */
		orig = gen_matrix_int(4, 4);
		orig[0][1] = orig[1][0] = 1;
		orig[0][3] = orig[3][0] = 1;
		orig[2][3] = orig[3][2] = 1;
		vs = adj_matrix_to_vertices(orig, 4, 1);
		recovered = vertices_to_adj_matrix(vs, 4);
		ok = 1;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (orig[i][j] != recovered[i][j])
					ok = 0;
		ASSERT(ok, "roundtrip adj_matrix -> vertices
			-> adj_matrix: matrix identical");
		free_matrix_int(orig, 4, 4);
		free_matrix_int(recovered, 4, 4);
		free_vertex_array(vs, 4);
	}
	TEST_GROUP("comp_adj_matrix");
	{
		/*
			* comp_adj_matrix: comp[i][j] = 1 - matrix[i][j] for i != j
			* Complement of empty = complete, complement of complete = empty
			*/
		empty = gen_matrix_int(3, 3);
		comp_empty = comp_adj_matrix(empty, 3);
		ASSERT(comp_empty != NULL, "comp_adj_matrix(empty): non-NULL");
		/* Off-diagonal should all be 1 */
		ASSERT(comp_empty[0][1] == 1 && comp_empty[1][0] == 1
			&& comp_empty[0][2] == 1 && comp_empty[1][2] == 1,
			"comp_adj_matrix(empty): complement of empty = complete (off-diag = 1)");
		/* Diagonal stays 0 */
		ASSERT(comp_empty[0][0] == 0 && comp_empty[1][1] == 0,
			"comp_adj_matrix(empty): diagonal stays 0");
		free_matrix_int(empty, 3, 3);
		free_matrix_int(comp_empty, 3, 3);
		/* Complement of K3 = empty */
		k3 = gen_matrix_int(3, 3);
		k3[0][1] = k3[1][0] = k3[0][2] = k3[2][0] = k3[1][2] = k3[2][1] = 1;
		comp_k3 = comp_adj_matrix(k3, 3);
		ASSERT(comp_k3[0][1] == 0 && comp_k3[1][2] == 0,
			"comp_adj_matrix(K3): complement of complete = empty (off-diag = 0)");
		free_matrix_int(k3, 3, 3);
		free_matrix_int(comp_k3, 3, 3);
	}
	TEST_GROUP("comp_adj_list");
	{
		/* Same as comp_adj_matrix but returns vertex list */
		m = gen_matrix_int(3, 3);
		m[0][1] = m[1][0] = 1; /* only 0-1 edge */
		comp = comp_adj_list(m, 3, 1);
		ASSERT(comp != NULL, "comp_adj_list: non-NULL");
		/* Complement undirected: missing edges are 0-2 and 1-2 */
		/* vs[0] should have edge to 2, vs[2] should have edges to 0 and 1 */
		ASSERT(get_target_edge(comp[0]->incidence, 2) != NULL,
			"comp_adj_list: vs[0] has edge to vs[2]");
		ASSERT(get_target_edge(comp[1]->incidence, 2) != NULL,
			"comp_adj_list: vs[1] has edge to vs[2]");
		ASSERT(get_target_edge(comp[0]->incidence, 1) == NULL,
			"comp_adj_list: vs[0] has no edge to vs[1] (was in original)");
		free_matrix_int(m, 3, 3);
		free_vertex_array(comp, 3);
	}
	TEST_GROUP("cost_matrix_to_vertices");
	{
		/* DBL_MAX/2 means no edge; other positive values = edges with costs */
		cm = gen_matrix_double(3, 3);
		cm[0][1] = cm[1][0] = 2.5;
		cm[0][2] = cm[2][0] = DBL_MAX / 2; /* no edge */
		cm[1][2] = cm[2][1] = 1.5;
		vs = cost_matrix_to_vertices(cm, 3, 1);
		ASSERT(vs != NULL, "cost_matrix_to_vertices: non-NULL");
		ASSERT(len_edge_list(vs[0]->incidence) == 1,
			"cost_matrix_to_vertices: vs[0] has 1 edge (DBL_MAX/2 skipped)");
		e01 = get_target_edge(vs[0]->incidence, 1);
		ASSERT(e01 != NULL && e01->cost == 2.5,
			"cost_matrix_to_vertices: edge 0->1 cost = 2.5");
		free_matrix_double(cm, 3, 3);
		free_vertex_array(vs, 3);
	}
	TEST_GROUP("dbcost_to_adj");
	{
		/*
			* dbcost_to_adj(int **cost, n): cost[i][j] > 0 -> 1, else 0
			*/
		cost = gen_matrix_int(3, 3);
		cost[0][1] = cost[1][0] = 5;
		cost[1][2] = cost[2][1] = 0; /* no edge */
		adj = dbcost_to_adj(cost, 3);
		ASSERT(adj != NULL, "dbcost_to_adj: non-NULL");
		ASSERT(adj[0][1] == 1, "dbcost_to_adj: positive cost -> 1");
		ASSERT(adj[1][0] == 1, "dbcost_to_adj: symmetric positive cost -> 1");
		ASSERT(adj[1][2] == 0, "dbcost_to_adj: zero cost -> 0");
		ASSERT(adj[0][0] == 0, "dbcost_to_adj: diagonal stays 0");
		free_matrix_int(cost, 3, 3);
		free_matrix_int(adj, 3, 3);
	}
	TEST_GROUP("edge_list_from_adj_list");
	{
		/* Undirected path 0-1-2: 2 edges */
		vs = create_vertex_array(3);
		add_undirected_edge(vs, 0, 1, 0, 1.0);
		add_undirected_edge(vs, 1, 2, 1, 2.0);
		el = edge_list_from_adj_list(vs, 3, 1);
		ASSERT(el != NULL, "edge_list_from_adj_list (undir): non-NULL");
		ASSERT(count_nodes(el) == 2,
			"edge_list_from_adj_list (undir): 2 unique edges");
		free_nodes(&el);
		free_vertex_array(vs, 3);
		/* Directed 0->1, 1->2: 2 edges */
		vs2 = create_vertex_array(3);
		add_directed_edge(vs2, 0, 1, 0, 1.0);
		add_directed_edge(vs2, 1, 2, 1, 2.0);
		el2 = edge_list_from_adj_list(vs2, 3, 0);
		ASSERT(count_nodes(el2) == 2,
			"edge_list_from_adj_list (dir): 2 directed edges");
		free_nodes(&el2);
		free_vertex_array(vs2, 3);
	}
}
