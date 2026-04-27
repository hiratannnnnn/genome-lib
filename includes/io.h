#ifndef IO_H
# define IO_H

# include "get_next_line.h"
# include "types.h"
# include <stdio.h>

/* ========= Input Processing ========= */

// read_matrix.c
int		**read_adj(int *n, char const *filename);
int		**read_list(int *n, char const *filename);
int		**read_matrix(int *a, int *b, char const *filename);
double	**read_double_matrix(int *r, int *c, char const *filename);

/* ========= Output ========= */

// write_to_file.c
int		write_adjacent_matrix(int **matrix, int n, char const *filename);
int		write_adjacent_list(int **matrix, int n, char const *filename);
int		write_path_node(Node *head, char const *filename);
int		write_double_matrix(double **matrix, int r, int c,
			char const *filename);
int		write_perm(int *perm, int n, char const *filename);
void	save_some_matrix(int n, double edge_prob, int **(*gen_some_mat)(int,
				double), char const *filename);

/* ========= Strings ========= */

// ft_split.c
char	**ft_split(char const *str, char *sep);

// ft_strtrim.c
char	*ft_strtrim(char const *str, char const *set);

#endif
