#ifndef UTILS_H
# define UTILS_H

# include <stdio.h>
# include "types.h"

/* ========= Edge Utilities ========= */

// utils_edges.c
Edge	*create_edge			(int id, int from, int to, double cost);
int		len_edge_list			(Edge *head);
void	add_directed_edge		(Vertex **vs, int from, int to, int edge_id, double cost);
void	add_undirected_edge		(Vertex **vs, int from, int to, int edge_id, double cost);
Edge	*get_last_edge			(Edge *head);
Edge	*get_target_edge		(Edge *head, int t);

/* ========= Vertex Utilities ========= */

// utils_vertex.c
Vertex	*create_vertex			(int id);
Vertex	**create_vertex_array	(int n);
void	reset_labels			(Vertex **vs, int n);
void	update_labels			(Node *node, int label);
void	sort_list_cyclic		(Vertex **vs, int n, int asc);

/* ========= Node Utilities ========= */

// utils_node.c
Node	*create_node			(void *ptr, NodeType type);
int		count_nodes				(Node *node);
Node	*get_last_node			(Node *node);
void	append_node				(Node **queue, Node *node);
Node	*pop_first_node			(Node **queue);
void	insert_node_after		(Node *pos, Node *new_node);
void	pop_last_node			(Node **head);
void	merge_nodes				(Node **a, Node **b);
double	cost_of_nodes			(Node *head);
void	enqueue_node			(Node **queue, Node *node);
Node	*dequeue_node			(Node **queue);
Edge	*node_get_edge			(const Node *node);
Vertex	*node_get_vertex		(const Node *node);
void	free_node				(Node *node);
void	free_nodes				(Node **node);

/* ========= Basic Utilities ========= */

// utils_basic.c
int		sum_matrix				(int **matrix, int r, int c);
int		sum_array				(int *arr, int n);
int		min_int					(int a, int b);
int		max_int					(int a, int b);
double	min_double				(double a, double b);
double	max_double				(double a, double b);

// utils_char.c
void	free_array_char			(char **ss, int n);
int		count_digit				(int number);

// utils_array.c
int		max_of_array			(int *arr, int n);
int		min_of_array			(int *arr, int n);
int		min_of_array_index		(int *arr, int n);

// utils_math.c
int		*rand_perm				(int n, int is_natural);

/* ========= Free Utilities ========= */

// utils_free.c
void	free_array_int			(int *ptr, int const n);
void	free_matrix_int			(int **matrix, int const r, int const c);
void	free_array_double		(double *ptr, int const c);
void	free_matrix_double		(double **matrix, int const r, int const c);
void	free_edge_list			(Edge *head);
void	free_vertex				(Vertex *vertex);
void	free_vertex_array		(Vertex **vs, int n);

/* ========= Print Utilities ========= */

// utils_print.c
void	print_matrix_int		(int **matrix, int r, int c);
void	fprint_array_int		(FILE *fp, int *arr, int n, int max_width);
void	print_array_int			(int *arr, int n, int max_width);
void	print_matrix_double		(double **matrix, int r, int c, int precision);
void	print_array_double		(double *arr, int c, int max_width, int precision);
void	print_vertex			(Vertex *v, int one_based);
void	print_vertices			(Vertex **vs, int n, int one_based);
void	print_edge_list			(Edge *head, int one_based);
void	print_node_path			(Node *head, int one_based);
void	print_array_node_path	(Node **ps, int n, int one_based);
void	print_node_edges		(Node *head, int one_based);
void	print_array_char		(char **ss, int n);
void	print_info				(void);

#endif
