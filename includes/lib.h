#ifndef LIB_H
# define LIB_H

#include <fcntl.h>
# include <math.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <ctype.h>
# include <string.h>
# include <time.h>
# include <unistd.h>
# include <float.h>
# include "types.h"

# include "get_next_line.h"

extern	size_t		mem;
extern	size_t		mem_peak;
extern	clock_t 	proc_start;
extern	clock_t		proc_end;

/* ========= Matrix Operations =========*/

// gen_matrix_int.c
int			**gen_matrix_int			(int r, int c);									// ok
int			**copy_matrix_int			(int **matrix, int r, int c); 					// ok

// gen_matrix_double.c
double 		**gen_matrix_double			(int r, int c);

// matrix_basic.c
void		identity_matrix				(int **matrix, int n);
void		permutation_matrix			(int **matrix, int n, int i, int j);
void		matrix_multiply				(int **A, int **B, int **C, int n);
void		swap_vertex					(int **matrix, int n, int i, int j);
void		shuffle_vertices			(int **matrix, int n);

/* ========= Graph Generation =========*/

// rand_bigraph_cost.c

int		 	max_bipartite_matching		(int **adj, int a, int b, int *match_to);
double 		**gen_rand_bigraph_cost		(int a, int b);

// rand_bigraph.c
int 		**gen_rand_bigraph			(int a, int b, double edge_prob);

// rand_digraph_cost.c
double 		**gen_rand_digraph_dbcost	(int n, double min, double max, double edge_prob);
int 		**gen_rand_digraph_intcost	(int n, int min, int max, double edge_prob);

// rand_digraph.c
int 		**gen_rand_digraph			(int n, double edge_prob);					// ok
int			**gen_rand_DAG				(int n, double edge_prob);					// ok

// rand_undigraph_cost.c
double 		**gen_rand_undigraph_dbcost(int n, double min, double max, double edge_prob);
int			**gen_rand_undigraph_intcost(int n, int min, int max, double edge_prob);

// rand_undigraph.c
int 		**gen_rand_undigraph		(int n, double edge_prob);					// ok, maybe name
int 		**gen_rand_conn_undigraph	(int n, double edge_prob);


// rand_tree.c
int			*gen_rand_prufer			(int n);					// ok, but maybe still can be generalized
int 		*count_degrees_from			(const int *prufer, int n);				// can be static
int 		**gen_rand_tree				(int n);										// ok
void		build_tree_from				(int **matrix, int *prufer, int *degree, int n);// ok

/* ========= Graph Analysis =========*/

// graph_analysis.c
int     	need_double_edge			(int *degree, int n);
int			count_edges					(int *degree, int n);
int			is_tree						(int **matrix, int n);					// ok, but when use?
void		compute_degrees				(int **matrix, int n, int *degree);		// ok
int			find_odd_vertices			(int *degree, int n, int *odd_list);	// ok, but too narrow?
void		compute_degrees_from_list	(Vertex **vs, int n, int *degree);		// ok?
int 		is_undigraph				(int **matrix, int n);

/* ========= Graph Format Conversion =========*/

// graph_conversion.c
Vertex		**adj_matrix_to_vertices	(int **matrix, int n, int undirected);	// ok
Vertex      **cost_matrix_to_vertices	(double **matrix, int n, int undirected);
int			**vertices_to_adj_matrix	(Vertex **vertices, int n);				// ok
int 		**comp_adj_matrix			(int **matrix, int n);
Vertex 		**comp_adj_list				(int **matrix, int n, int undir);
int			**dbcost_to_adj				(int **cost, int n);

// utils_edge_list.c
Node		*edge_list_from_adj_list	(Vertex **vs, int n, int is_undir);

// line_graph.c
int			**line_graph_from_adj_list	(Vertex **vs, int n, char ***names, int *size, int is_undir); // ok?

/* ========= Input Processing =========*/

// read_matrix.c
int 		**read_adj			(int *n, char const *filename);						// ok
int 		**read_list			(int *n, char const *filename);
int 		**read_matrix		(int *a, int *b, char const *filename);
double 		**read_double_matrix(int *r, int *c, char const *filename);			// ok

/* ========= Sorting =========*/
// cmp.c
int 	cmp_int_desc		(int a, int b);
int 	cmp_int_asc			(int a, int b);
int 	cmp_int_bogo		(int a, int b);
int 	cmp_double_desc		(double a, double b);
int 	cmp_double_asc		(double a, double b);
int 	cmp_double_bogo		(double a, double b);
int 	cmp_edge_cost		(Edge *e1, Edge *e2);
int 	cmp_edge_cost_id	(Edge *e1, Edge *e2);

// sort_edge_list.c
void	sort_list				(Edge **head, int (*cmp)(int, int));
void	sort_nodes_vertex_id	(Node **head, int (*cmp)(int, int));
void	sort_nodes_edge_cost	(Node **head, int (*cmp)(double, double));
void	sort_list_by_degree		(Edge **head, int *degree, int (*cmp)(int, int));


/* ========= Utilities =========*/

// utils_edges.c
Edge		*create_edge			(int id, int from, int to, double cost);					// ok
int 		len_edge_list			(Edge *head);								// ok, scattered functions
void		add_directed_edge		(Vertex **vs, int from, int to, int edge_id, double cost);	// edge_id?
void		add_undirected_edge		(Vertex **vs, int from, int to, int edge_id, double cost);	// edge_id?
Edge 		*get_last_edge			(Edge *head);
Edge 		*get_target_edge		(Edge *head, int t);

// utils_vertex.c
Vertex		*create_vertex			(int id);									// ok
Vertex		**create_vertex_array	(int n);									// ok
void 		reset_labels			(Vertex **vs, int n);
void		update_labels			(Node *node, int label);
void 		sort_list_cyclic		(Vertex **vs, int n, int asc);

/* ========= Euler Tour Utilities =========*/

// utils_basic.c
int			sum_matrix		(int **matrix, int r, int c);						// ok
int			sum_array		(int *arr, int n);									// ok
int			min_int			(int a, int b);
int 		max_int			(int a, int b);
double		min_double		(double a, double b);
double 		max_double		(double a, double b);

// utils_char.c
void	 	free_array_char			(char **ss, int n);
int 		count_digit				(int number);

// utils_free.c
void		free_array_int		(int *ptr, int const n);
void		free_matrix_int		(int **matrix, int const r, int const c);
void		free_array_double	(double *ptr, int const c);
void		free_matrix_double	(double **matrix, int const r, int const c);
void		free_edge_list		(Edge *head);
void		free_vertex			(Vertex *vertex);
void		free_vertex_array	(Vertex **vs, int n);

// utils_node.c
Node 		*create_node			(void *ptr, NodeType type);
int 		count_nodes				(Node *node);
Node 		*get_last_node			(Node *node);
void		append_node				(Node **queue, Node *node);
Node		*pop_first_node			(Node **queue);
void		insert_node_after		(Node *pos, Node *new_node);
void		pop_last_node			(Node **head);
void		merge_nodes				(Node **a, Node **b);
double		cost_of_nodes			(Node *head);
void		enqueue_node			(Node **queue, Node *node);
Node		*dequeue_node			(Node **queue);
Edge		*node_get_edge			(const Node *node);
Vertex 		*node_get_vertex		(const Node *node);
void		free_node				(Node *node);
void		free_nodes				(Node **node);

// utils_math.c
int			*rand_perm		(int n);

// utils_array.c
int			max_of_array		(int *arr, int n);
int			min_of_array		(int *arr, int n);
int			min_of_array_index	(int *arr, int n);

// utils_print.c
void 		print_matrix_int	(int **matrix, int r, int c);
void		print_array_int		(int *arr, int n, int max_width);
void		print_matrix_double			(double **matrix, int r, int c, int precision);
void 		print_array_double			(double *arr, int c, int max_width, int precision);
void		print_vertex		(Vertex *v, int one_based);
void		print_vertices		(Vertex **vs, int n, int one_based);
void		print_edge_list		(Edge *head, int one_based);
void 		print_node_path			(Node *head, int one_based);
void		print_array_node_path	(Node **ps, int n, int one_based);
void 		print_node_edges		(Node *head, int one_based);
void		print_array_char	(char **ss, int n);
void		print_info			(	);

// write_to_file.c
int			write_adjacent_matrix	(int **matrix, int n, char const *filename);	// ok
int 		write_adjacent_list		(int **matrix, int n, char const *filename);	// ok
int 		write_path_node			(Node *head, char const *filename);
int 		write_double_matrix		(double **matrix, int r, int c, char const *filename);
int 		write_perm				(int *perm, int n, char const *filename);
void 		save_some_matrix		(int n, double edge_prob,						// ok
	int **(*gen_some_mat)(int, double), char const *filename);

/* ========= Strings ========= */

// ft_split.c
char 		**ft_split			(char const *str, char *sep);

// ft_strtrim.c
char 		*ft_strtrim			(char const *str, char const *set);


/* ========= Memory Allocation ========= */

// memory_dump.c
void		dump_memory_hex		(const void *ptr, int size, int offset);
void		dump_memory_detailed(const void *ptr, int size, const char *label);

// xmalloc.c
void		*xmalloc			(size_t size);
void		*xcalloc			(int n, size_t size_each);
void		xfree				(void *ptr, size_t size);
void		*xrealloc			(void *ptr, size_t old_size, size_t new_size);

/* ========= Permutation Operations =========*/

/*
 * Permutation convention: 0-indexed, values in {0, ..., n-1}.
 * perm[i] = j  means element j is at position i.
 */

// permutation.c
void    identity_permutation        (int *perm, int n);
void    copy_permutation            (int *src, int *dst, int n);
int     is_identity_permutation     (int *perm, int n);
void    inverse_permutation         (int *perm, int *inv, int n);
void    compose_permutation         (int *p, int *q, int *res, int n);
int     count_breakpoints           (int *perm, int n);
void    block_interchange           (int *arr, int n, int i, int j, int k, int l);
void    prefix_block_interchange    (int *arr, int n, int j, int k, int l);

#endif
