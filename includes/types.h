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

typedef enum
{
	NODE_TYPE_EDGE,
    NODE_TYPE_VERTEX,
}			NodeType;

typedef struct Node
{
	void *ptr;
	NodeType type;
	struct Node *prev;
	struct Node *next;
}				Node;

typedef int (*HeapCmp)(const void *a, const void *b);

/*
 * Breakpoint graph G(π) for an unsigned linear permutation (0-indexed, values 0..n-1).
 *
 * Extended permutation: pbar[0]=0, pbar[i+1]=perm[i]+1 (i=0..n-1), pbar[n+1]=n+1.
 * Each value v in {0,...,n+1} is split into two vertices: v_left=2v, v_right=2v+1.
 * Total vertices: 2*(n+2).
 *
 * Black arcs (identity adjacency): v_right -- (v+1)_left, for v=0..n.
 * Grey arcs (permutation adjacency): pbar[i]_right -- pbar[i+1]_left, for i=0..n.
 *
 * The graph decomposes into vertex-disjoint alternating cycles.
 */
typedef struct
{
    int  n;          /* length of the original permutation */
    int *grey;       /* grey[v]     = grey-arc neighbor of v   (size: 2*(n+2)) */
    int *black;      /* black[v]    = black-arc neighbor of v  (size: 2*(n+2)) */
    int *cycle_id;   /* cycle_id[v] = index of v's cycle       (size: 2*(n+2)) */
    int  num_cycles; /* total number of alternating cycles */
} BreakpointGraph;

typedef struct
{
    void **array;
    int size;
    int capacity;
    HeapCmp cmp;
}       Heap;

#endif
