#ifndef MATRIX_H
# define MATRIX_H

/* ========= Matrix Operations ========= */

// generate_matrix_int.c
int		**gen_matrix_int		(int r, int c);
int		**copy_matrix_int		(int **matrix, int r, int c);

// generate_matrix_double.c
double	**gen_matrix_double		(int r, int c);

// matrix_basic.c
void	identity_matrix			(int **matrix, int n);
void	permutation_matrix		(int **matrix, int n, int i, int j);
void	matrix_multiply			(int **A, int **B, int **C, int n);
void	swap_vertex				(int **matrix, int n, int i, int j);
void	shuffle_vertices		(int **matrix, int n);

#endif
