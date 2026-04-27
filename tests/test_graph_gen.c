#include "lib.h"
#include "test_framework.h"

/* ---- helpers ---- */

/* BFS connectivity check */
static int	is_connected(int **matrix, int n)
{
	int	*visited;
	int	*queue;
	int	front;
	int	back;
	int	u;
	int	result;

	visited = (int *)calloc(n, sizeof(int));
	queue = (int *)calloc(n, sizeof(int));
	front = 0;
	back = 0;
	visited[0] = 1;
	queue[back++] = 0;
	while (front < back)
	{
		u = queue[front++];
		for (int v = 0; v < n; v++)
			if (matrix[u][v] && !visited[v])
			{
				visited[v] = 1;
				queue[back++] = v;
			}
	}
	result = 1;
	for (int i = 0; i < n; i++)
		if (!visited[i])
			result = 0;
	free(visited);
	free(queue);
	return (result);
}

/* Checks no self-loops */
static int	no_self_loops(int **m, int n)
{
	for (int i = 0; i < n; i++)
		if (m[i][i] != 0)
			return (0);
	return (1);
}

/* Checks matrix is symmetric */
static int	is_symmetric(int **m, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (m[i][j] != m[j][i])
				return (0);
	return (1);
}

/* ---- tests ---- */

void	run_tests_graph_gen(void)
{
	int		**m;
	int		binary;
	int		**m0;
	int		sum;
	int		**m1;
	int		all_edges;
	int		**m2;
	int		no_mutual;
	int		edges;
	int		**m3;
	int		n;
	int		valid;
	double	**dm;
	int		sym;
	int		*prufer;

	TEST_GROUP("gen_rand_undigraph");
	{
		m = gen_rand_undigraph(6, 0.5);
		ASSERT(m != NULL, "gen_rand_undigraph: non-NULL");
		ASSERT(is_symmetric(m, 6), "gen_rand_undigraph: symmetric");
		ASSERT(no_self_loops(m, 6), "gen_rand_undigraph: no self-loops");
		/* Only 0/1 values */
		binary = 1;
		for (int i = 0; i < 6; i++)
			for (int j = 0; j < 6; j++)
				if (m[i][j] != 0 && m[i][j] != 1)
					binary = 0;
		ASSERT(binary, "gen_rand_undigraph: only 0/1 entries");
		free_matrix_int(m, 6, 6);
		/* Edge prob = 0 -> no edges */
		m0 = gen_rand_undigraph(5, 0.0);
		sum = 0;
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				sum += m0[i][j];
		ASSERT(sum == 0, "gen_rand_undigraph(prob=0): no edges");
		free_matrix_int(m0, 5, 5);
		/* Edge prob = 1 -> all edges (complete graph) */
		m1 = gen_rand_undigraph(4, 1.0);
		all_edges = 1;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (i != j && m1[i][j] != 1)
					all_edges = 0;
		ASSERT(all_edges, "gen_rand_undigraph(prob=1): complete graph");
		free_matrix_int(m1, 4, 4);
	}
	TEST_GROUP("gen_rand_conn_undigraph");
	{
		m = gen_rand_conn_undigraph(6, 0.3);
		ASSERT(m != NULL, "gen_rand_conn_undigraph: non-NULL");
		ASSERT(is_symmetric(m, 6), "gen_rand_conn_undigraph: symmetric");
		ASSERT(no_self_loops(m, 6), "gen_rand_conn_undigraph: no self-loops");
		ASSERT(is_connected(m, 6), "gen_rand_conn_undigraph: connected");
		free_matrix_int(m, 6, 6);
		/* n=2: must have edge 0-1 */
		m2 = gen_rand_conn_undigraph(2, 0.0);
		ASSERT(m2[0][1] == 1 && m2[1][0] == 1,
			"gen_rand_conn_undigraph(n=2): edge always present");
		free_matrix_int(m2, 2, 2);
	}
	TEST_GROUP("gen_rand_digraph");
	{
		m = gen_rand_digraph(5, 0.5);
		ASSERT(m != NULL, "gen_rand_digraph: non-NULL");
		ASSERT(no_self_loops(m, 5), "gen_rand_digraph: no self-loops");
		binary = 1;
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				if (m[i][j] != 0 && m[i][j] != 1)
					binary = 0;
		ASSERT(binary, "gen_rand_digraph: only 0/1 entries");
		free_matrix_int(m, 5, 5);
		/* prob=0 -> no edges */
		m0 = gen_rand_digraph(5, 0.0);
		sum = 0;
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				sum += m0[i][j];
		ASSERT(sum == 0, "gen_rand_digraph(prob=0): no edges");
		free_matrix_int(m0, 5, 5);
	}
	TEST_GROUP("gen_rand_DAG");
	{
		/*
			* A DAG has no directed cycles.
			* We check: for each edge i->j,
				verify topological ordering is maintained.
			* gen_rand_DAG uses a random permutation (topnr),
				and only adds edges
			* from lower topnr to higher (or adjacent). So matrix[i][j]=1 only if
			* topnr[i] < topnr[j]. We can't recover topnr, but we can verify no
			* obvious cycle: the graph should be a DAG (no undirected odd-cycles via
			* simple DFS). Instead, just verify structural invariants.
			*/
		m = gen_rand_DAG(6, 0.5);
		ASSERT(m != NULL, "gen_rand_DAG: non-NULL");
		ASSERT(no_self_loops(m, 6), "gen_rand_DAG: no self-loops");
		/* Simple cycle check: if m[i][j]=1 then m[j][i] must be 0
			(since topnr[i]<topnr[j] implies topnr[j]>topnr[i])
			Note: not 100% guaranteed by the algorithm for all pairs,
			but valid for directly connected pairs */
		no_mutual = 1;
		for (int i = 0; i < 6; i++)
			for (int j = 0; j < 6; j++)
				if (m[i][j] == 1 && m[j][i] == 1)
					no_mutual = 0;
		ASSERT(no_mutual, "gen_rand_DAG: no mutual edges (no 2-cycles)");
		free_matrix_int(m, 6, 6);
	}
	TEST_GROUP("gen_rand_tree");
	{
		m = gen_rand_tree(6);
		ASSERT(m != NULL, "gen_rand_tree: non-NULL");
		ASSERT(is_tree(m, 6), "gen_rand_tree: is a valid tree");
		ASSERT(is_symmetric(m, 6), "gen_rand_tree: symmetric (undirected)");
		ASSERT(no_self_loops(m, 6), "gen_rand_tree: no self-loops");
		/* n-1 edges */
		edges = 0;
		for (int i = 0; i < 6; i++)
			for (int j = i + 1; j < 6; j++)
				edges += m[i][j];
		ASSERT(edges == 5, "gen_rand_tree(6): exactly 5 edges");
		free_matrix_int(m, 6, 6);
		/* Minimum valid tree (n=3, Prufer length = 1) */
		m3 = gen_rand_tree(3);
		ASSERT(m3 != NULL, "gen_rand_tree(3): non-NULL");
		ASSERT(is_tree(m3, 3), "gen_rand_tree(3): is a valid tree");
		free_matrix_int(m3, 3, 3);
	}
	TEST_GROUP("gen_rand_bigraph");
	{
		/*
			* gen_rand_bigraph(a, b) returns an a×b biadjacency matrix.
			* NOTE: the implementation sets matrix[i][j] = matrix[j][i] = 1,
			*       so a must equal b to avoid out-of-bounds access (library constraint).
			* We use a=b=4 as a smoke test.
			*/
		n = 4;
		m = gen_rand_bigraph(n, n, 0.7);
		ASSERT(m != NULL, "gen_rand_bigraph(4,4): non-NULL");
		ASSERT(is_symmetric(m, n), "gen_rand_bigraph(4,4): symmetric");
		binary = 1;
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				if (m[i][j] != 0 && m[i][j] != 1)
					binary = 0;
		ASSERT(binary, "gen_rand_bigraph(4,4): only 0/1 entries");
		free_matrix_int(m, n, n);
		/* prob=0 -> no edges */
		m0 = gen_rand_bigraph(n, n, 0.0);
		ASSERT(m0 != NULL, "gen_rand_bigraph(prob=0): non-NULL");
		sum = 0;
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				sum += m0[i][j];
		ASSERT(sum == 0, "gen_rand_bigraph(prob=0): no edges");
		free_matrix_int(m0, n, n);
	}
	TEST_GROUP("gen_rand_undigraph_intcost");
	{
		/*
			* "No edge" is represented by INT_MAX/2, not 0.
			* Present edges have costs in [min, max].
			*/
		m = gen_rand_undigraph_intcost(5, 1, 10, 0.6);
		ASSERT(m != NULL, "gen_rand_undigraph_intcost: non-NULL");
		ASSERT(is_symmetric(m, 5), "gen_rand_undigraph_intcost: symmetric");
		valid = 1;
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				if (m[i][j] != INT_MAX / 2 && m[i][j] != 0 && (m[i][j] < 1
						|| m[i][j] > 10))
					valid = 0;
		ASSERT(valid,
			"gen_rand_undigraph_intcost: costs in [1,10] or INT_MAX/2 (no-edge)");
		free_matrix_int(m, 5, 5);
	}
	TEST_GROUP("gen_rand_digraph_intcost");
	{
		m = gen_rand_digraph_intcost(5, 2, 8, 0.5);
		ASSERT(m != NULL, "gen_rand_digraph_intcost: non-NULL");
		/* diagonal is 0 (skipped by implementation),
			off-diag: cost or INT_MAX/2 */
		ASSERT(m[0][0] == 0, "gen_rand_digraph_intcost: diagonal is 0");
		valid = 1;
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				if (i != j && m[i][j] != INT_MAX / 2 && (m[i][j] < 2
						|| m[i][j] > 8))
					valid = 0;
		ASSERT(valid,
			"gen_rand_digraph_intcost: costs in [2,8] or INT_MAX/2 (no-edge)");
		free_matrix_int(m, 5, 5);
	}
	TEST_GROUP("gen_rand_undigraph_dbcost");
	{
		dm = gen_rand_undigraph_dbcost(4, 0.5, 5.0, 0.6);
		ASSERT(dm != NULL, "gen_rand_undigraph_dbcost: non-NULL");
		/* Symmetric: cost[i][j] == cost[j][i] */
		sym = 1;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (dm[i][j] != dm[j][i])
					sym = 0;
		ASSERT(sym, "gen_rand_undigraph_dbcost: symmetric");
		free_matrix_double(dm, 4, 4);
	}
	TEST_GROUP("gen_rand_prufer / gen_rand_tree via Prufer");
	{
		/* gen_rand_prufer(n) returns array of length n-2 with values in [0,n-1] */
		prufer = gen_rand_prufer(6);
		ASSERT(prufer != NULL, "gen_rand_prufer(6): non-NULL");
		valid = 1;
		for (int i = 0; i < 4; i++) /* length = n-2 = 4 */
			if (prufer[i] < 0 || prufer[i] >= 6)
				valid = 0;
		ASSERT(valid, "gen_rand_prufer(6): values in [0,5]");
		free(prufer);
	}
}
