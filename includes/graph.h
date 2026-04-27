#ifndef GRAPH_H
# define GRAPH_H

# include "types.h"

/* ========= Graph Generation ========= */

// random_bigraph_cost.c
double	**gen_rand_bigraph_cost		(int a, int b);

// random_bigraph.c
int		**gen_rand_bigraph			(int a, int b, double edge_prob);

// random_digraph_cost.c
double	**gen_rand_digraph_dbcost	(int n, double min, double max, double edge_prob);
int		**gen_rand_digraph_intcost	(int n, int min, int max, double edge_prob);

// random_digraph.c
int		**gen_rand_digraph			(int n, double edge_prob);
int		**gen_rand_DAG				(int n, double edge_prob);

// random_undigraph_cost.c
double	**gen_rand_undigraph_dbcost	(int n, double min, double max, double edge_prob);
int		**gen_rand_undigraph_intcost(int n, int min, int max, double edge_prob);

// random_undigraph.c
int		**gen_rand_undigraph		(int n, double edge_prob);
int		**gen_rand_conn_undigraph	(int n, double edge_prob);

// random_tree.c
int		*gen_rand_prufer			(int n);
int		*count_degrees_from			(const int *prufer, int n);
int		**gen_rand_tree				(int n);
void	build_tree_from				(int **matrix, int *prufer, int *degree, int n);

/* ========= Graph Analysis ========= */

// graph_analysis.c
int		need_double_edge			(int *degree, int n);
int		count_edges					(int *degree, int n);
int		is_tree						(int **matrix, int n);
void	compute_degrees				(int **matrix, int n, int *degree);
int		find_odd_vertices			(int *degree, int n, int *odd_list);
void	compute_degrees_from_list	(Vertex **vs, int n, int *degree);
int		is_undigraph				(int **matrix, int n);

/* ========= Graph Format Conversion ========= */

// graph_conversion.c
Vertex	**adj_matrix_to_vertices	(int **matrix, int n, int undirected);
Vertex	**cost_matrix_to_vertices	(double **matrix, int n, int undirected);
int		**vertices_to_adj_matrix	(Vertex **vertices, int n);
int		**comp_adj_matrix			(int **matrix, int n);
Vertex	**comp_adj_list				(int **matrix, int n, int undir);
int		**dbcost_to_adj				(int **cost, int n);

// utils_edge_list.c
Node	*edge_list_from_adj_list	(Vertex **vs, int n, int is_undir);

// line_graph.c
int		**line_graph_from_adj_list	(Vertex **vs, int n, char ***names, int *size, int is_undir);

#endif
