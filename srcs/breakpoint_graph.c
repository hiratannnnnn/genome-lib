/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   breakpoint_graph.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:10:43 by thirata           #+#    #+#             */
/*   Updated: 2026/04/11 16:24:02 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

/*
 * Index-based breakpoint graph for a 1-indexed permutation arr[0..n-1]
 * (values 1..n).
 *
 * Vertex numbering: bp[v] IS vertex v.
 *   right(0)   = vertex 0          (left sentinel)
 *   left(k)    = vertex 2k-1       (k = 1..n)
 *   right(k)   = vertex 2k         (k = 1..n)
 *   left(n+1)  = vertex 2n+1       (right sentinel)
 *   size       = 2*(n+1)
 *
 * Grey arcs (identity / sorted adjacency):
 *   right(k) -- left(k+1)  for k = 0..n
 *   => bp[v].grey = v+1 if v is even
 *   => bp[v].grey = v-1 if v is odd
 *
 * Black arcs (permutation adjacency):
 *   right(pbar[i]) -- left(pbar[i+1])  for i = 0..n
 *   where pbar[0]=0, pbar[1..n]=arr[0..n-1], pbar[n+1]=n+1.
 *   => bp[2*pbar[i]].black     = 2*pbar[i+1]-1
 *   => bp[2*pbar[i+1]-1].black = 2*pbar[i]
 */

void print_bp_graph(t_bp_graph *bp, int size)
{
    int v;

    v = 0;
    while (v < size)
    {
        printf("%d\t%c", v, v == size - 1 ? '\n' : ' ');
        v++;
    }
    v = 0;
    while (v < size)
    {
        printf("%d\t%c", bp[v].grey, v == size - 1 ? '\n' : ' ');
        v++;
    }
    v = 0;
    while (v < size)
    {
        printf("%d\t%c", bp[v].black, v == size - 1 ? '\n' : ' ');
        v++;
    }
    v = 0;
    while (v < size)
    {
        printf("%d\t%c", bp[v].cycle_id, v == size - 1 ? '\n' : ' ');
        v++;
    }
}

/*
 * Grey arcs: identity adjacency.
 * Even vertex v (= right of some value): grey neighbor = v+1 (the next left).
 * Odd vertex v  (= left of some value):  grey neighbor = v-1 (the prev right).
 */
static void set_greys(t_bp_graph *bp, int size)
{
    int v;

    v = 0;
    while (v < size)
    {
        if (v % 2 == 0)
            bp[v].grey = v + 1;
        else
            bp[v].grey = v - 1;
        v++;
    }
}

/*
 * Black arcs: permutation adjacency.
 * Extended permutation pbar: pbar[0]=0, pbar[1..n]=arr[0..n-1], pbar[n+1]=n+1.
 * Arc i connects right(pbar[i]) = vertex 2*pbar[i]
 *              to  left(pbar[i+1]) = vertex 2*pbar[i+1]-1.
 */
static void set_blacks(t_bp_graph *bp, int size, int *arr, int n)
{
    int *pbar;
    int  i;

    (void)size;
    pbar = (int *)xmalloc((size_t)(n + 2) * sizeof(int));
    pbar[0] = 0;
    i = 0;
    while (i < n)
    {
        pbar[i + 1] = arr[i];
        i++;
    }
    pbar[n + 1] = n + 1;

    i = 0;
    while (i <= n)
    {
        bp[2 * pbar[i]].black       = 2 * pbar[i + 1] - 1;
        bp[2 * pbar[i + 1] - 1].black = 2 * pbar[i];
        i++;
    }

    xfree(pbar, (size_t)(n + 2) * sizeof(int));
}

/*
 * Cycle decomposition: follow black then grey alternately.
 * Direct indexing — no linear search needed.
 */
static void seek_cycles(t_bp_graph *bp, int start, int id)
{
    int cur;

    cur = start;
    while (bp[cur].cycle_id == -1)
    {
        bp[cur].cycle_id = id;
        cur = bp[cur].black;
        bp[cur].cycle_id = id;
        cur = bp[cur].grey;
    }
}

static void clear_cycle_id(t_bp_graph*bp, int size)
{
    int v;

    v = 0;
    while (v < size)
    {
        bp[v].cycle_id = -1;
        v++;
    }
}

int set_cycle_id(t_bp_graph *bp, int size)
{
    int v;
    int id;

    clear_cycle_id(bp, size);
    v = 0;
    id = 0;
    while (v < size)
    {
        if (bp[v].cycle_id == -1)
        {
            seek_cycles(bp, v, id);
            id++;
        }
        v++;
    }
    return (id);
}

void    bp_graph_init(t_bp_graph *bp, int size, int *arr, int n)
{
    set_greys(bp, size);
    set_blacks(bp, size, arr, n);
    set_cycle_id(bp, size);
}
