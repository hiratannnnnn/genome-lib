#ifndef SORT_H
# define SORT_H

# include "types.h"

/* ========= Comparators ========= */

// cmp.c
int		cmp_int_desc		(int a, int b);
int		cmp_int_asc			(int a, int b);
int		cmp_int_bogo		(int a, int b);
int		cmp_double_desc		(double a, double b);
int		cmp_double_asc		(double a, double b);
int		cmp_double_bogo		(double a, double b);
int		cmp_edge_cost		(Edge *e1, Edge *e2);
int		cmp_edge_cost_id	(Edge *e1, Edge *e2);

/* ========= Sorting ========= */

// sort_edge_list.c
void	sort_list				(Edge **head, int (*cmp)(int, int));
void	sort_nodes_vertex_id	(Node **head, int (*cmp)(int, int));
void	sort_nodes_edge_cost	(Node **head, int (*cmp)(double, double));
void	sort_list_by_degree		(Edge **head, int *degree, int (*cmp)(int, int));

#endif
