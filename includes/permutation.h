#ifndef PERMUTATION_H
# define PERMUTATION_H

# include "types.h"

/*
 * Permutation convention: 0-indexed, values in {0, ..., n-1}.
 * perm[i] = j  means element j is at position i.
 */

/* ========= Permutation Operations ========= */

// permutation.c
void	identity_permutation(int *perm, int n, int is_natural);
void	copy_permutation(int *src, int *dst, int n);
int		is_identity_permutation(int *perm, int n, int is_natural);
void	inverse_permutation(int *perm, int *inv, int n);
void	compose_permutation(int *p, int *q, int *res, int n);
int		count_breakpoints(int *perm, int n);
int		find_value(int *perm, int n, int val);
int		first_breakpoint_position(int *perm, int n);

// generate_permutation.c
void	rev_array_int(int *begin, int *end);
int		next_permutation(int *arr, int n);

// breakpoint_graph.c
void	print_bp_graph(t_bp_graph *bp, int size);
int		set_cycle_id(t_bp_graph *bp, int size);
void	bp_graph_init(t_bp_graph *bp, int size, int *arr, int n);

#endif
