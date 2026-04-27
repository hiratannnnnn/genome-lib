/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_graph.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:11:27 by thirata           #+#    #+#             */
/*   Updated: 2026/04/10 08:11:30 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

static char	*name_v_of_LG(int from, int to, int is_undir)
{
	char	*str;
	int		len_from;
	int		len_to;
	char	digit;

	int size, i, temp;
	len_from = count_digit((unsigned int)from);
	len_to = count_digit((unsigned int)to);
	size = len_from + 2 + len_to;
	str = (char *)xmalloc(sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	i = len_from - 1;
	temp = from;
	while (i >= 0)
	{
		digit = temp % 10 + '0';
		str[i] = digit;
		temp /= 10;
		i--;
	}
	if (is_undir)
	{
		str[len_from] = '-';
		str[len_from + 1] = '-';
	}
	else
	{
		str[len_from] = '-';
		str[len_from + 1] = '>';
	}
	temp = to;
	i = size - 1;
	while (i >= size - len_to)
	{
		digit = temp % 10 + '0';
		str[i] = digit;
		temp /= 10;
		i--;
	}
	str[size] = '\0';
	return (str);
}

/**
 * @brief make adjacency matrix of LINE GRAPH of given matrix. (adj. list)
 *
 * @param vs Adjacency List
 * @param n
 * @param is_undir 1, dir 0
 */
int	**line_graph_from_adj_list(Vertex **vs, int n, char ***names, int *size,
		int is_undir)
{
	Node	**node_array;
	int		**matrix;

	Node *head, *ni;
	Edge *ei, *ej;
	int len, i, j;
	head = edge_list_from_adj_list(vs, n, is_undir);
	len = count_nodes(head);
	*size = len;
	matrix = gen_matrix_int(len, len);
	*names = (char **)xmalloc(sizeof(char *) * len);
	if (!*names)
	{
		free_nodes(&head);
		return (NULL);
	}
	if (!matrix)
	{
		free_array_char(*names, len);
		free_nodes(&head);
		return (NULL);
	}
	node_array = xmalloc(sizeof(Node *) * len);
	if (!node_array)
	{
		free_array_char(*names, len);
		free_matrix_int(matrix, len, len);
		free_nodes(&head);
		return (NULL);
	}
	ni = head;
	for (i = 0; i < len && ni; i++, ni = ni->next)
	{
		node_array[i] = ni;
		ei = (Edge *)ni->ptr;
		(*names)[i] = name_v_of_LG(ei->from, ei->to, is_undir);
		if (!(*names)[i])
		{
			free_array_char(*names, len);
			free_matrix_int(matrix, len, len);
			free_nodes(&head);
			return (NULL);
		}
	}
	for (i = 0; i < len; i++)
	{
		ei = (Edge *)node_array[i]->ptr;
		for (j = 0; j < len; j++)
		{
			if (i == j)
				continue ;
			ej = (Edge *)node_array[j]->ptr;
			if (is_undir)
			{
				if (ei->from == ej->from || ei->from == ej->to
					|| ei->to == ej->from || ei->to == ej->to)
					matrix[i][j] = matrix[j][i] = 1;
			}
			else
			{
				if (ei->to == ej->from)
					matrix[i][j] = 1;
			}
		}
	}
	xfree(node_array, sizeof(Node *) * len);
	free_nodes(&head);
	return (matrix);
}
