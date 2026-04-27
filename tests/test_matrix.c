#include "lib.h"
#include "test_framework.h"

/* ---- helpers ---- */

static int	matrix_all_zero_int(int **m, int r, int c)
{
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			if (m[i][j] != 0)
				return (0);
	return (1);
}

static int	matrix_all_zero_double(double **m, int r, int c)
{
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			if (m[i][j] != 0.0)
				return (0);
	return (1);
}

static int	is_identity(int **m, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if ((i == j && m[i][j] != 1) || (i != j && m[i][j] != 0))
				return (0);
	return (1);
}

/* ---- tests ---- */

void	run_tests_matrix(void)
{
	int		**m;
	int		**m2;
	int		**orig;
	int		**copy;
	int		eq;
	double	**dm;
	int		**id;
	int		**id1;
	int		**pm;
	int		**pm2;
	int		**A;
	int		**B;
	int		**C;
	int		**sv;
	int		**sv2;
	int		**mat;
	int		edge_count;
	int		sym;
	int		no_self;

	TEST_GROUP("gen_matrix_int");
	{
		m = gen_matrix_int(3, 4);
		ASSERT(m != NULL, "gen_matrix_int(3,4): returns non-NULL");
		ASSERT(matrix_all_zero_int(m, 3, 4),
			"gen_matrix_int: all elements zero");
		free_matrix_int(m, 3, 4);
		m2 = gen_matrix_int(1, 1);
		ASSERT(m2 != NULL, "gen_matrix_int(1,1): edge case returns non-NULL");
		ASSERT(m2[0][0] == 0, "gen_matrix_int(1,1): element is zero");
		free_matrix_int(m2, 1, 1);
	}
	TEST_GROUP("copy_matrix_int");
	{
		orig = gen_matrix_int(3, 3);
		orig[0][0] = 1;
		orig[0][1] = 2;
		orig[0][2] = 3;
		orig[1][0] = 4;
		orig[1][1] = 5;
		orig[1][2] = 6;
		orig[2][0] = 7;
		orig[2][1] = 8;
		orig[2][2] = 9;
		copy = copy_matrix_int(orig, 3, 3);
		ASSERT(copy != NULL, "copy_matrix_int: returns non-NULL");
		eq = 1;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (copy[i][j] != orig[i][j])
					eq = 0;
		ASSERT(eq, "copy_matrix_int: values match original");
		copy[0][0] = 99;
		ASSERT(orig[0][0] == 1,
			"copy_matrix_int: deep copy (modifying copy leaves original intact)");
		free_matrix_int(orig, 3, 3);
		free_matrix_int(copy, 3, 3);
	}
	TEST_GROUP("gen_matrix_double");
	{
		dm = gen_matrix_double(4, 5);
		ASSERT(dm != NULL, "gen_matrix_double(4,5): returns non-NULL");
		ASSERT(matrix_all_zero_double(dm, 4, 5),
			"gen_matrix_double: all elements 0.0");
		free_matrix_double(dm, 4, 5);
	}
	TEST_GROUP("identity_matrix");
	{
		id = gen_matrix_int(4, 4);
		identity_matrix(id, 4);
		ASSERT(is_identity(id, 4), "identity_matrix(4): diagonal=1, rest=0");
		free_matrix_int(id, 4, 4);
		id1 = gen_matrix_int(1, 1);
		identity_matrix(id1, 1);
		ASSERT(id1[0][0] == 1, "identity_matrix(1): single element is 1");
		free_matrix_int(id1, 1, 1);
	}
	TEST_GROUP("permutation_matrix");
	{
		/* permutation_matrix: starts with identity, then swaps rows i and j */
		pm = gen_matrix_int(3, 3);
		permutation_matrix(pm, 3, 0, 1);
		/* After swapping rows 0 and 1 of identity:
			row0 = [0,1,0], row1 = [1,0,0], row2 = [0,0,1] */
		ASSERT(pm[0][1] == 1 && pm[0][0] == 0,
			"permutation_matrix(0,1): row 0 swapped");
		ASSERT(pm[1][0] == 1 && pm[1][1] == 0,
			"permutation_matrix(0,1): row 1 swapped");
		ASSERT(pm[2][2] == 1, "permutation_matrix(0,1): row 2 unchanged");
		free_matrix_int(pm, 3, 3);
		/* Out-of-bounds: no change expected */
		pm2 = gen_matrix_int(3, 3);
		identity_matrix(pm2, 3);
		permutation_matrix(pm2, 3, -1, 1); /* should be no-op */
		ASSERT(is_identity(pm2, 3),
			"permutation_matrix: out-of-bounds i is no-op");
		free_matrix_int(pm2, 3, 3);
	}
	TEST_GROUP("matrix_multiply");
	{
		/* I * I = I */
		A = gen_matrix_int(3, 3);
		B = gen_matrix_int(3, 3);
		C = gen_matrix_int(3, 3);
		identity_matrix(A, 3);
		identity_matrix(B, 3);
		matrix_multiply(A, B, C, 3);
		ASSERT(is_identity(C, 3), "matrix_multiply: I * I = I");
		/* Known product: [[1,2,0],[0,1,0],[0,0,1]] * I = same */
		A[0][0] = 1;
		A[0][1] = 2;
		A[0][2] = 0;
		A[1][0] = 0;
		A[1][1] = 1;
		A[1][2] = 0;
		A[2][0] = 0;
		A[2][1] = 0;
		A[2][2] = 1;
		identity_matrix(B, 3);
		matrix_multiply(A, B, C, 3);
		ASSERT(C[0][0] == 1 && C[0][1] == 2 && C[1][1] == 1, "matrix_multiply: A
			* I = A");
		/* [[1,1],[1,1]]
			* [[1,1],[1,1]] = [[2,2],[2,2]] (2x2 via 3x3 top-left) */
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				A[i][j] = B[i][j] = 0;
		A[0][0] = 1;
		A[0][1] = 1;
		A[1][0] = 1;
		A[1][1] = 1;
		B[0][0] = 1;
		B[0][1] = 1;
		B[1][0] = 1;
		B[1][1] = 1;
		matrix_multiply(A, B, C, 3);
		ASSERT(C[0][0] == 2 && C[0][1] == 2 && C[1][0] == 2 && C[1][1] == 2,
			"matrix_multiply: [[1,1],[1,1]]^2 = [[2,2],[2,2]]");
		free_matrix_int(A, 3, 3);
		free_matrix_int(B, 3, 3);
		free_matrix_int(C, 3, 3);
	}
	TEST_GROUP("swap_vertex");
	{
		/*
			* Graph: 0-1 only (no 1-2, no 0-2)
			* matrix[0][1]=matrix[1][0]=1, rest=0
			* swap_vertex(matrix, 3, 0, 2):
			*   row swap (0<->2): matrix[2][1]=1, matrix[1][2]... wait,
				let's trace:
			*   after row swap: matrix[2][1]=1 (from old row0),
				matrix[1][0]=1 still
			*   after col swap: matrix[1][0] and matrix[1][2] swap: matrix[1][2]=1,
				matrix[1][0]=0
			* Result: matrix[2][1]=1, matrix[1][2]=1
			*/
		sv = gen_matrix_int(3, 3);
		sv[0][1] = sv[1][0] = 1;
		swap_vertex(sv, 3, 0, 2);
		ASSERT(sv[2][1] == 1 && sv[1][2] == 1,
			"swap_vertex: edge (0-1) moves to (2-1) after swap(0,2)");
		ASSERT(sv[0][1] == 0 && sv[1][0] == 0,
			"swap_vertex: original position cleared");
		ASSERT(sv[0][2] == 0 && sv[2][0] == 0,
			"swap_vertex: no spurious edge between swapped vertices");
		free_matrix_int(sv, 3, 3);
		/* Swapping a vertex with itself: no change */
		sv2 = gen_matrix_int(3, 3);
		sv2[0][1] = sv2[1][0] = 1;
		sv2[1][2] = sv2[2][1] = 1;
		swap_vertex(sv2, 3, 1, 1);
		ASSERT(sv2[0][1] == 1 && sv2[1][2] == 1,
			"swap_vertex(i,i): matrix unchanged");
		free_matrix_int(sv2, 3, 3);
	}
	TEST_GROUP("shuffle_vertices");
	{
		/*
			* shuffle_vertices permutes vertex order.
			* The total number of edges must be preserved.
			* Use a 4-vertex path: 0-1, 1-2, 2-3 (3 edges).
			*/
		mat = gen_matrix_int(4, 4);
		mat[0][1] = mat[1][0] = 1;
		mat[1][2] = mat[2][1] = 1;
		mat[2][3] = mat[3][2] = 1;
		shuffle_vertices(mat, 4);
		edge_count = 0;
		for (int i = 0; i < 4; i++)
			for (int j = i + 1; j < 4; j++)
				edge_count += mat[i][j];
		ASSERT(edge_count == 3,
			"shuffle_vertices: edge count preserved (3 edges)");
		/* symmetry preserved */
		sym = 1;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (mat[i][j] != mat[j][i])
					sym = 0;
		ASSERT(sym, "shuffle_vertices: matrix stays symmetric");
		/* no self-loops */
		no_self = 1;
		for (int i = 0; i < 4; i++)
			if (mat[i][i] != 0)
				no_self = 0;
		ASSERT(no_self, "shuffle_vertices: no self-loops");
		free_matrix_int(mat, 4, 4);
	}
}
