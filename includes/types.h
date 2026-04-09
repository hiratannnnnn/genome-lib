#ifndef TYPES_H
# define TYPES_H

/* ========= Data Structure =========*/

typedef struct 	Edge
{
    int         id;			// edge_id
    int         from;   	// vertex from
    int         to;     	// vertex to
    double		cost;		// for any extra uses
    struct Edge *next;		// for edge list
}               Edge;

typedef struct 	Vertex
{
    int         id;			// vertex id
    int         label;
    Edge        *incidence;	// incidence list (edge list)
}               Vertex;

typedef struct 	EdgeNode
{
    Edge		*edge;
    struct EdgeNode *prev;		// prev node
    struct EdgeNode *next;		// next node
}               EdgeNode;

typedef struct 	PathNode
{
    Vertex          *v;
    struct PathNode *prev;	// prev node
    struct PathNode *next;	// next node
}				PathNode;

typedef enum
{
	NODE_TYPE_EDGE,
    NODE_TYPE_VERTEX,
    ROUND,
    SQUARE
}			NodeType;

typedef struct Node
{
	void *ptr;
	NodeType type;
	struct Node *prev;
	struct Node *next;
}				Node;

typedef int (*HeapCmp)(const void *a, const void *b);

typedef struct
{
    void **array;
    int size;
    int capacity;
    HeapCmp cmp;
}       Heap;

#endif
