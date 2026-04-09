#include "lib.h"

PathNode 	*create_pathnode(Vertex *vertex)
{
	PathNode 	*node;

	node = (PathNode *)xmalloc(sizeof(PathNode));
	if (!node)
		return NULL;
	node->v = vertex;
	node->prev = NULL;
	node->next = NULL;
	return (node);
}

void		append_pathnode(PathNode **head, PathNode *new_pathnode)
{
	PathNode	*last;

	if (!head || !new_pathnode)
		return ;
	if (*head == NULL)
	{
		*head = new_pathnode;
		return ;
	}
	last = get_last_pathnode(*head);
	last->next = new_pathnode;			// append to the last
	new_pathnode->prev = last;
}

void		insert_pathnode_after(PathNode *pos, PathNode *new_pathnode)
{
	if (!pos || !new_pathnode)
		return ;
	new_pathnode->next = pos->next;
	new_pathnode->prev = pos;
	if (pos->next)
		pos->next->prev = new_pathnode;
	pos->next = new_pathnode;
}

PathNode	*get_last_pathnode(PathNode *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return head;
}

PathNode 	*pathnode_pop_first(PathNode **head)
{
	PathNode *first;

	if (!head || !*head)
		return (NULL);
	first = *head;
	*head = first->next;
	if (*head)
		(*head)->prev = NULL;
	return first;
}

void		pathnode_pop_last(PathNode **head)
{
	PathNode *last;

	if (!head || !*head)
		return ;
	last = get_last_pathnode(*head);
	if (last->prev)
		last->prev->next = NULL;
	else
		*head = NULL;
	xfree(last, sizeof(PathNode));
}

int	count_pathnodes(PathNode *head)
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

void merge_pathnode(PathNode **a, PathNode **b)
{
	if (*a != NULL && *a == *b)
	{
		printf("Do not pass the same pointers\n");
		return ;
	}

	PathNode *a_last;

	a_last = get_last_pathnode(*a);
	if (a_last)
	{
		if (*b)
		{
			(*b)->prev = a_last;
			a_last->next = (*b);
		}
	}
	else
		*a = *b;
	*b = NULL;
}

void 	free_pathnode(PathNode **head)
{
	PathNode *cur;
    PathNode *tmp;

	cur = *head;
    while (cur)
    {
        tmp = cur;
		cur = cur->next;
        xfree(tmp, sizeof(PathNode));
    }
	*head = NULL;
}
