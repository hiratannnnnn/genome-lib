#include "lib.h"
#include "test_framework.h"

void	run_tests_graph_analysis(void)
{
	int		**m;
	int		degree[3] = {0, 0, 0};
	int		deg_path[] = {1, 2, 1};
	int		deg_k3[] = {2, 2, 2};
	int		deg_empty[] = {0, 0, 0, 0};
	int		**tree;
	int		**disc;
	int		**single;
	int		odd_list[3] = {0, 0, 0};
	int		cnt;
	int		odd_k3[3] = {-1, -1, -1};
	int		cnt2;
	int		deg_star[] = {4, 1, 1, 1, 1};
	int		odd_star[5];
	int		cnt3;
	int		**sym;
	int		**zero;
	Vertex	**vs;
	int		deg_need[] = {3, 2, 2, 1};
	int		deg_noneed[] = {2, 2, 2, 2};
	int		deg_odd[] = {2, 2, 2};

	TEST_GROUP("compute_degrees");
	{
		/*
			* Undirected path: 0-1-2
			* matrix[0][1]=matrix[1][0]=1, matrix[1][2]=matrix[2][1]=1
			* degrees: [1, 2, 1]
			*/
		m = gen_matrix_int(3, 3);
		m[0][1] = m[1][0] = 1;
		m[1][2] = m[2][1] = 1;
		compute_degrees(m, 3, degree);
		ASSERT(degree[0] == 1,
			"compute_degrees: vertex 0 degree = 1 (path graph)");
		ASSERT(degree[1] == 2,
			"compute_degrees: vertex 1 degree = 2 (path graph)");
		ASSERT(degree[2] == 1,
			"compute_degrees: vertex 2 degree = 1 (path graph)");
		/* Complete graph K3: each vertex has degree 2 */
		m[0][2] = m[2][0] = 1;
		compute_degrees(m, 3, degree);
		ASSERT(degree[0] == 2 && degree[1] == 2 && degree[2] == 2,
			"compute_degrees: K3 all degrees = 2");
		free_matrix_int(m, 3, 3);
	}
	TEST_GROUP("count_edges");
	{
		/* Path 0-1-2: degrees [1,2,1], edges = (1+2+1)/2 = 2 */
		ASSERT(count_edges(deg_path, 3) == 2, "count_edges: path graph = 2");
		/* K3: degrees [2,2,2], edges = 6/2 = 3 */
		ASSERT(count_edges(deg_k3, 3) == 3, "count_edges: K3 = 3");
		/* No edges */
		ASSERT(count_edges(deg_empty, 4) == 0, "count_edges: empty graph = 0");
	}
	TEST_GROUP("is_tree");
	{
		/* Tree: path 0-1-2-3 */
		tree = gen_matrix_int(4, 4);
		tree[0][1] = tree[1][0] = 1;
		tree[1][2] = tree[2][1] = 1;
		tree[2][3] = tree[3][2] = 1;
		ASSERT(is_tree(tree, 4) == 1, "is_tree: path graph (tree)");
		/* Cycle: not a tree */
		tree[0][3] = tree[3][0] = 1; /* adds edge 0-3, creates cycle */
		ASSERT(is_tree(tree, 4) == 0, "is_tree: cycle graph (not a tree)");
		free_matrix_int(tree, 4, 4);
		/* Disconnected: not a tree */
		disc = gen_matrix_int(4, 4);
		disc[0][1] = disc[1][0] = 1;
		disc[2][3] = disc[3][2] = 1;
		ASSERT(is_tree(disc, 4) == 0,
			"is_tree: disconnected graph (not a tree)");
		free_matrix_int(disc, 4, 4);
		/* Single vertex: is a tree */
		single = gen_matrix_int(1, 1);
		ASSERT(is_tree(single, 1) == 1, "is_tree: single vertex is a tree");
		free_matrix_int(single, 1, 1);
	}
	TEST_GROUP("find_odd_vertices");
	{
		/* Path 0-1-2: vertices 0 and 2 have odd degree (1) */
		cnt = find_odd_vertices(deg_path, 3, odd_list);
		ASSERT(cnt == 2,
			"find_odd_vertices: path graph has 2 odd-degree vertices");
		ASSERT(odd_list[0] == 0, "find_odd_vertices: first odd vertex is 0");
		ASSERT(odd_list[1] == 2, "find_odd_vertices: second odd vertex is 2");
		/* K3: all even degrees (2) */
		cnt2 = find_odd_vertices(deg_k3, 3, odd_k3);
		ASSERT(cnt2 == 0, "find_odd_vertices: K3 has 0 odd-degree vertices");
		/* Star graph with 4 leaves: center has degree 4 (even),
			leaves have degree 1 (odd) */
		cnt3 = find_odd_vertices(deg_star, 5, odd_star);
		ASSERT(cnt3 == 4,
			"find_odd_vertices: star graph has 4 odd-degree vertices (leaves)");
	}
	TEST_GROUP("is_undigraph");
	{
		/* Symmetric matrix -> undirected */
		sym = gen_matrix_int(3, 3);
		sym[0][1] = sym[1][0] = 1;
		sym[1][2] = sym[2][1] = 1;
		ASSERT(is_undigraph(sym, 3) == 1, "is_undigraph: symmetric matrix = 1");
		/* Asymmetric -> directed */
		sym[0][2] = 1; /* sym[2][0] still = 0 */
		ASSERT(is_undigraph(sym, 3) == 0,
			"is_undigraph: asymmetric matrix = 0");
		free_matrix_int(sym, 3, 3);
		/* Zero matrix is trivially symmetric */
		zero = gen_matrix_int(4, 4);
		ASSERT(is_undigraph(zero, 4) == 1, "is_undigraph: zero matrix = 1");
		free_matrix_int(zero, 4, 4);
	}
	TEST_GROUP("compute_degrees_from_list");
	{
		/*
			* Undirected graph: 0-1, 1-2
			* Each undirected edge appears in both directions in incidence lists.
			* compute_degrees_from_list counts degree[to]++ for each directed edge stored.
			* vs[0]: edge 0->1  → degree[1]++
			* vs[1]: edge 1->0, edge 1->2  → degree[0]++, degree[2]++
			* vs[2]: edge 2->1  → degree[1]++
			* Result: degree[0]=1, degree[1]=2, degree[2]=1
			*/
		vs = create_vertex_array(3);
		add_undirected_edge(vs, 0, 1, 0, 1.0);
		add_undirected_edge(vs, 1, 2, 1, 1.0);
		degree[0] = 0;
		degree[1] = 0;
		degree[2] = 0;
		compute_degrees_from_list(vs, 3, degree);
		ASSERT(degree[0] == 1,
			"compute_degrees_from_list: vertex 0 degree = 1");
		ASSERT(degree[1] == 2,
			"compute_degrees_from_list: vertex 1 degree = 2");
		ASSERT(degree[2] == 1,
			"compute_degrees_from_list: vertex 2 degree = 1");
		free_vertex_array(vs, 3);
	}
	TEST_GROUP("need_double_edge");
	{
		/*
			* need_double_edge returns 1 if:
			*   n is even AND any vertex has degree == n-1
			* Returns 0 otherwise.
			*/
		/* n=4 (even), vertex 0 has degree 3 (=n-1=3) -> need double edge */
		ASSERT(need_double_edge(deg_need, 4) == 1,
			"need_double_edge: even n - vertex with degree n-1 -> 1");
		/* n=4 (even), no vertex has degree 3 -> no double edge needed */
		ASSERT(need_double_edge(deg_noneed, 4) == 0,
			"need_double_edge: even n - no vertex with degree n-1 -> 0");
		/* n=3 (odd) -> always 0 */
		ASSERT(need_double_edge(deg_odd, 3) == 0,
			"need_double_edge: odd n -> always 0");
	}
}
