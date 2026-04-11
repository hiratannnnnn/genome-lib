/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:12:38 by thirata           #+#    #+#             */
/*   Updated: 2026/04/11 22:52:26 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

static int count_max_digits(int **matrix, int r, int c)
{
	int i, j, max, digit;

	max = 1;
	for (i = 0; i < r; i++)
		for (j = 0; j < c; j++)
		{
			digit = count_digit(matrix[i][j]);
			if (digit > max)
				max = digit;
		}
	return max;
}

void print_matrix_int(int **matrix, int r, int c)
{
	int i;
	int max_width;

	if (!matrix)
	{
		printf("matrix is NULL\n");
		return;
	}
	max_width = count_max_digits(matrix, r, c);
	for (i = 0; i < r; i++)
	{
		if (!matrix[i])
			return;
		print_array_int(matrix[i], c, max_width);
	}
}

void	fprint_array_int(FILE *fp, int *arr, int n, int max_width)
{
	int i;

	for (i = 0; i < n; i++)
	{
		fprintf(fp, "%*d", max_width, arr[i]);
		fprintf(fp, (i == n - 1) ? "\n" : " ");
	}
}

/**
 * @param max_width 0, if you wanna disable digit alignment
 */
void print_array_int(int *arr, int n, int max_width)
{
	int i;
	for (i = 0; i < n; i++)
	{
		printf("%*d", max_width, arr[i]);
		printf((i == n - 1) ? "\n" : " ");
	}
}

static int count_max_double_width(double **matrix, int r, int c, int precision)
{
    int i, j, max_int_width, int_width;
    double value;

    max_int_width = 1;
    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            value = matrix[i][j];
            if (value < 0)
            {
                value = -value;
                int_width = 1;
            }
            else
                int_width = 0;
            if (value < 1.0)
                int_width += 1;  // Just "0" before decimal
            else
                while (value >= 1.0)
                {
                    int_width++;
                    value /= 10.0;
                }
            if (int_width > max_int_width)
                max_int_width = int_width;
        }
    }
    return max_int_width + 1 + precision;
}

void print_matrix_double(double **matrix, int r, int c, int precision)
{
    int i;
    int max_width;

    if (!matrix)
    {
        printf("matrix is NULL\n");
        return;
    }

    if (precision <= 0)
        precision = 3;
    max_width = count_max_double_width(matrix, r, c, precision);
    for (i = 0; i < r; i++)
    {
        if (!matrix[i])
            return;
        print_array_double(matrix[i], c, max_width, precision);
    }
}

void print_array_double(double *arr, int n, int max_width, int precision)
{
    int i;
    char format[20];

    if (precision <= 0)
		precision = 3;
	if (max_width >= 9)
		max_width = 9;
    if (max_width <= 0)
    	sprintf(format, "%%.%df", precision);
    else
        sprintf(format, "%%%d.%df", max_width, precision);
    for (i = 0; i < n; i++)
    {
        if (arr[i] == DBL_MAX / 2)
			printf("%*s", max_width > 0 ? max_width : 9, "DBL_MAX/2");
		else
			printf(format, arr[i]);
        printf((i == n - 1) ? "\n" : " ");
    }
}

void	print_vertex(Vertex *v, int one_based)
{
	if (!v)
		return ;
	if (one_based != 0 && one_based != 1)
	{
		printf("one_based must be 0 or 1.\n");
		return ;
	}
	printf("vId: %d", v->id + one_based);
	print_edge_list(v->incidence, one_based);
}

void	print_vertices(Vertex **vs, int n, int one_based)
{
	int	i;
	if (one_based != 0 && one_based != 1)
	{
		printf("one_based must be 0 or 1.\n");
		return ;
	}

	i = 0;
	if (!vs)
		return ;
	while (i < n)
	{
		print_vertex(vs[i], one_based);
		i++;
	}
	if (one_based)
		printf("Printed in 1-based indexing.\n");
}

void	print_edge_list(Edge *head, int one_based)
{
	Edge	*cur;
	if (one_based != 0 && one_based != 1)
	{
		printf("one_based must be 0 or 1.\n");
		return ;
	}

	cur = head;
	while (cur)
	{
		printf(" -> ");
		printf("%d", cur->to + one_based);
		cur = cur->next;
	}
	printf("\n");
}

void 	print_node_path(Node *head, int one_based)
{
	Edge *e;
	Vertex *v;

	if (one_based != 0 && one_based != 1)
	{
		printf("one_based must be 0 or 1.\n");
		return ;
	}
	while (head)
	{
		if (head->type == NODE_TYPE_VERTEX)
		{
			v = (Vertex *)head->ptr;
			printf(" -> %d", v->id + one_based);
		}
		else if (head->type == NODE_TYPE_EDGE)
		{
			e = (Edge *)head->ptr;
			printf(" -> %d", e->to + one_based);
		}
		head = head->next;
	}
	printf("\n");
}

void	print_array_node_path(Node **ps, int n, int one_based)
{
	int i;
	if (one_based != 0 && one_based != 1)
	{
		printf("one_based must be 0 or 1.\n");
		return ;
	}

	for (i = 0; i < n; i++)
	{
		printf("%d :", i + one_based);
		print_node_path(ps[i], one_based);
	}
	if (one_based)
		printf("Printed in 1-based indexing.\n");
	printf("\n");
}

void print_node_edges(Node *head, int one_based)
{
	Edge *e;
	double cost;

	if (one_based != 0 && one_based != 1)
	{
		printf("one_based must be 0 or 1.\n");
		return ;
	}
	cost = 0.0;
	while (head)
	{
		if (head->type == NODE_TYPE_EDGE)
		{
			e = (Edge *)head->ptr;
			printf("%d - %d\n", e->from + one_based, e->to + one_based);
			cost += e->cost;
		}
		head = head->next;
	}
	if (cost)
		printf("cost was %f\n", cost);
	if (one_based)
		printf("Printed in 1-based indexing.\n");
	printf("\n");
}

void	print_array_char(char **ss, int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		printf("%s\n", ss[i]);
	}
}

void	print_info()
{
	double used_time;

	proc_end = clock();
	used_time = ((double) (proc_end - proc_start)) / CLOCKS_PER_SEC;
	printf("mem_peak: %d\n", (int)mem_peak);
	printf("remaining mem: %d\n", (int)mem);
	printf("proccess time: %f\n", used_time);
	proc_start = clock();
}
