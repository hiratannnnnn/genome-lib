#include "lib.h"

void	sort_pathnode(PathNode **head, int (*cmp)(int, int))
{
	PathNode *ptr1, *lptr, *prev, *tmp;
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
			if (cmp(ptr1->v->id, ptr1->next->v->id) > 0)
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

void sort_edgenode_cost(EdgeNode **head, int (*cmp)(double, double))
{
	EdgeNode *ptr1, *lptr, *prev, *tmp;
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
			if (cmp(ptr1->edge->cost, ptr1->next->edge->cost) > 0)
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
