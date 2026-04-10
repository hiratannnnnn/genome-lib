/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_edge_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:12:19 by thirata           #+#    #+#             */
/*   Updated: 2026/04/10 08:12:20 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

void	sort_nodes_vertex_id(Node **head, int (*cmp)(int, int))
{
	Node *ptr1, *lptr, *prev, *tmp;
	int swapped;

	if (!head || !*head)
		return ;
	lptr = NULL;
	do {
		swapped = 0;
		ptr1 = *head;
		prev = NULL;
		while (ptr1->next != lptr)
		{
			if (cmp(((Vertex *)ptr1->ptr)->id,
					((Vertex *)ptr1->next->ptr)->id) > 0)
			{
				tmp = ptr1->next;
				ptr1->next = tmp->next;
				if (tmp->next)
					tmp->next->prev = ptr1;
				tmp->next = ptr1;
				ptr1->prev = tmp;
				if (prev)
				{
					prev->next = tmp;
					tmp->prev = prev;
				}
				else
				{
					*head = tmp;
					tmp->prev = NULL;
				}
				swapped = 1;
				prev = tmp;
			}
			else
			{
				prev = ptr1;
				ptr1 = ptr1->next;
			}
		}
		lptr = ptr1;
	} while (swapped);
}

void	sort_nodes_edge_cost(Node **head, int (*cmp)(double, double))
{
	Node *ptr1, *lptr, *prev, *tmp;
	int swapped;

	if (!head || !*head)
		return ;
	lptr = NULL;
	do {
		swapped = 0;
		ptr1 = *head;
		prev = NULL;
		while (ptr1->next != lptr)
		{
			if (cmp(((Edge *)ptr1->ptr)->cost,
					((Edge *)ptr1->next->ptr)->cost) > 0)
			{
				tmp = ptr1->next;
				ptr1->next = tmp->next;
				if (tmp->next)
					tmp->next->prev = ptr1;
				tmp->next = ptr1;
				ptr1->prev = tmp;
				if (prev)
				{
					prev->next = tmp;
					tmp->prev = prev;
				}
				else
				{
					*head = tmp;
					tmp->prev = NULL;
				}
				swapped = 1;
				prev = tmp;
			}
			else
			{
				prev = ptr1;
				ptr1 = ptr1->next;
			}
		}
		lptr = ptr1;
	} while (swapped);
}

void	sort_list(Edge **head, int (*cmp)(int, int))
{
	Edge *ptr1, *lptr, *prev, *tmp;
	int swapped;

	if (!head || !*head)
		return ;
	lptr = NULL;
	do
	{
		swapped = 0;
		ptr1 = *head;
		prev = NULL;
		while (ptr1->next != lptr)
		{
			if (cmp(ptr1->to, ptr1->next->to) > 0)
			{
				tmp = ptr1->next;
				ptr1->next = tmp->next;
				tmp->next = ptr1;
				if (prev)
					prev->next = tmp;
				else
					*head = tmp;
				swapped = 1;
				prev = tmp;
			}
			else
			{
				prev = ptr1;
				ptr1 = ptr1->next;
			}
		}
		lptr = ptr1;
	} while (swapped);
}

void	sort_list_by_degree(Edge **head, int *degree, int (*cmp)(int, int))
{
	Edge *ptr1, *lptr, *tmp, *prev;
	int swapped;

	if (!head || !*head)
		return ;
	lptr = NULL;
	do
	{
		swapped = 0;
		ptr1 = *head;
		prev = NULL;
		while (ptr1->next != lptr)
		{
			if (cmp(degree[ptr1->to], degree[ptr1->next->to]) > 0)
			{
				tmp = ptr1->next;
				ptr1->next = tmp->next;
				tmp->next = ptr1;
				if (prev)
					prev->next = tmp;
				else
					*head = tmp;
				swapped = 1;
				prev = tmp;
			}
			else
			{
				prev = ptr1;
				ptr1 = ptr1->next;
			}
		}
		lptr = ptr1;
	} while (swapped);
}
