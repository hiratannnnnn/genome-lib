#include "lib.h"

EdgeNode	*create_edgenode(Edge *edge)
{
	EdgeNode	*node;

	node = (EdgeNode *)xmalloc(sizeof(EdgeNode));
	if (!node)
		return (NULL);
	node->edge = edge;
	node->prev = NULL;
	node->next = NULL;
	return (node);
}

void	append_edgenode(EdgeNode **head, EdgeNode *new_node)
{
	EdgeNode	*last;

	if (!head || !new_node)
		return ;
	if (*head == NULL)
	{
		*head = new_node;
		return ;
	}
	last = get_last_edgenode(*head);
	last->next = new_node;
	new_node->prev = last;
}

void	insert_node_after(EdgeNode *pos, EdgeNode *new_node)
{
	if (!pos || !new_node)
		return ;
	new_node->next = pos->next;
	new_node->prev = pos;
	if (pos->next)
		pos->next->prev = new_node;
	pos->next = new_node;
}

EdgeNode 	*get_last_edgenode(EdgeNode *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return head;
}

int	count_edgenodes(EdgeNode *head)
{
	int len;

	len = 0;
	while (head)
	{
		len++;
		head = head->next;
	}
	return len;
}

double cost_of_edgenodes(EdgeNode *head)
{
	double cost;

	cost = 0.0;
	while (head)
	{
		cost += head->edge->cost;
		head = head->next;
	}
	return cost;
}

void	merge_edgenode(EdgeNode **a, EdgeNode **b)
{
	EdgeNode *a_last;
	EdgeNode *b_head;

	if (*a != NULL && *a == *b)
	{
		printf("Do not pass the same pointers.\n");
		return ;
	}
	a_last = get_last_edgenode(*a);
	b_head = *b;
	if (a_last)
	{
		a_last->next = b_head;
		if (b_head)
			b_head->prev = a_last;
	}
	else
		*a = b_head;
	*b = NULL;
}


EdgeNode	*edgenode_pop_first(EdgeNode **head)
{
	EdgeNode *first;

	if (!head || !*head)
		return (NULL);
	first = *head;
	// printf("[edgenode_pop_first] first: %d -> %d\n", first->edge->from, first->edge->to);
	*head = first->next;
	if (*head)
		(*head)->prev = NULL;
	return first;
}

void	free_edgenode(EdgeNode **head)
{
	EdgeNode	*cur;
	EdgeNode	*next;

	cur = *head;
	while (cur)
	{
		next = cur->next;
		xfree(cur, sizeof(EdgeNode));
		cur = next;
	}
	*head = NULL;
}
