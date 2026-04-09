#include "lib.h"

Node 	*create_node(void *ptr, NodeType type)
{
	Node *node;
	if (!ptr)
	{
		printf("pointer is pointing NULL.\n");
		return NULL;
	}

	node = (Node *)xmalloc(sizeof(Node));
	if (!node)
		return NULL;
	node->ptr = ptr;
	node->type = type;
	node->prev = NULL;
	node->next = NULL;
	return node;
}

int 	get_node_length(Node *node)
{
	int count;
	count = 0;
	while (node)
	{
		count++;
		node = node->next;
	}
	return count;
}

Node 	*get_last_node(Node *node)
{
	if (!node)
		return NULL;
	while (node->next)
		node = node->next;
	return node;
}

void	append_node(Node **queue, Node *node)
{
	Node *last;

	last = get_last_node(*queue);
	if (last)
	{
		last->next = node;
		node->prev = last;
	}
	else
		*queue = node;
}

Node	*pop_first_node(Node **queue)
{
	Node *first;

	first = *queue;
	if (first)
	{
		*queue = first->next;
		if (first->next)
			first->next->prev = NULL;
		first->next = NULL;
		first->prev = NULL;
	}
	return first;
}

void	enqueue_node(Node **queue, Node *node)
{
	append_node(queue, node);
}

Node	*dequeue_node(Node **queue)
{
	return pop_first_node(queue);
}

Edge	*node_get_edge(const Node *node)
{
	if (!node || node->type != NODE_TYPE_EDGE)
	{
		printf("Expected Edge, got type %d\n",
				node ? (int)node->type : -1);
		return NULL;
	}
	return (Edge *)(node->ptr);
}

Vertex 	*node_get_vertex(const Node *node)
{
	if (!node || node->type != NODE_TYPE_VERTEX)
	{
		printf("Expected Vertex, got type %d\n",
				node ? (int)node->type : -1);
		return NULL;
	}
	return (Vertex *)node->ptr;
}

void	free_node(Node *node)
{
	if (!node)
		return ;

	xfree(node, sizeof(Node));
}

void	free_nodes(Node **node)
{
	Node *head, *next;

	head = *node;
	while (head)
	{
		next = head->next;
		free_node(head);
		head = next;
	}
	*node = NULL;
}
