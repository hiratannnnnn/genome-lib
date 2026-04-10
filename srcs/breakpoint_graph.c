/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   breakpoint_graph.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:10:43 by thirata           #+#    #+#             */
/*   Updated: 2026/04/10 08:10:44 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

/*
 * Build the breakpoint graph for a 0-indexed permutation (values 0..n-1).
 *
 * Extended permutation pbar:
 *   pbar[0]   = 0
 *   pbar[i+1] = perm[i]+1  for i = 0..n-1
 *   pbar[n+1] = n+1
 *
 * Vertices: value v -> v_left = 2v, v_right = 2v+1. Total: 2*(n+2).
 *
 * Black arcs: black[2v+1] = 2(v+1)  and  black[2(v+1)] = 2v+1  for v=0..n.
 * Grey arcs:  grey[2*pbar[i]+1] = 2*pbar[i+1]  and  grey[2*pbar[i+1]] = 2*pbar[i]+1  for i=0..n.
 *
 * Vertices 0_left (=0) and (n+1)_right (=2n+3) are not covered by any arc;
 * they remain with grey[v]=black[v]=-1 and cycle_id[v]=-1.
 */
BreakpointGraph *bg_build(int *perm, int n)
{
    BreakpointGraph *bg;
    int             *pbar;
    int              sz, v, i;

    bg           = (BreakpointGraph *)xmalloc(sizeof(BreakpointGraph));
    bg->n        = n;
    sz           = 2 * (n + 2);
    bg->grey     = (int *)xmalloc((size_t)sz * sizeof(int));
    bg->black    = (int *)xmalloc((size_t)sz * sizeof(int));
    bg->cycle_id = (int *)xmalloc((size_t)sz * sizeof(int));

    for (i = 0; i < sz; i++)
    {
        bg->grey[i]     = -1;
        bg->black[i]    = -1;
        bg->cycle_id[i] = -1;
    }
    bg->num_cycles = 0;

    /* Build extended permutation */
    pbar = (int *)xmalloc((size_t)(n + 2) * sizeof(int));
    pbar[0] = 0;
    for (i = 0; i < n; i++)
        pbar[i + 1] = perm[i] + 1;
    pbar[n + 1] = n + 1;

    /* Black arcs: v_right -- (v+1)_left, for v = 0..n */
    for (v = 0; v <= n; v++)
    {
        bg->black[2 * v + 1]   = 2 * (v + 1);
        bg->black[2 * (v + 1)] = 2 * v + 1;
    }

    /* Grey arcs: pbar[i]_right -- pbar[i+1]_left, for i = 0..n */
    for (i = 0; i <= n; i++)
    {
        bg->grey[2 * pbar[i] + 1]  = 2 * pbar[i + 1];
        bg->grey[2 * pbar[i + 1]]  = 2 * pbar[i] + 1;
    }

    xfree(pbar, (size_t)(n + 2) * sizeof(int));
    return bg;
}

/*
 * Decompose the breakpoint graph into alternating cycles.
 * Sets cycle_id[v] for all active vertices and sets num_cycles.
 *
 * Each cycle is traced by alternating grey then black arcs:
 *   cur -> grey[cur] -> black[grey[cur]] -> ...  until back at start.
 * Inactive vertices (grey[v]==-1) are skipped and keep cycle_id==-1.
 */
void bg_decompose_cycles(BreakpointGraph *bg)
{
    int sz, v, cid, cur, grey_nb;

    sz  = 2 * (bg->n + 2);
    cid = 0;

    for (v = 0; v < sz; v++)
        bg->cycle_id[v] = -1;

    for (v = 0; v < sz; v++)
    {
        if (bg->cycle_id[v] != -1 || bg->grey[v] == -1)
            continue;
        cur = v;
        do {
            bg->cycle_id[cur] = cid;
            grey_nb = bg->grey[cur];
            bg->cycle_id[grey_nb] = cid;
            cur = bg->black[grey_nb];
        } while (cur != v);
        cid++;
    }
    bg->num_cycles = cid;
}

/*
 * Return 1 if the black arc incident to pbar-value v belongs to the leftmost cycle.
 *
 * The leftmost cycle is the cycle of vertex 0_right = 1.
 * The black arc for value v connects 2v+1 (v_right) and 2(v+1) ((v+1)_left).
 *
 * v is in the pbar range {0,...,n+1}.
 * In practice, call with v = perm[i]+1 for a 0-indexed permutation value perm[i].
 */
int bg_black_arc_in_leftmost(BreakpointGraph *bg, int v)
{
    int leftmost_cid;

    leftmost_cid = bg->cycle_id[1]; /* vertex 0_right = 1 */
    return bg->cycle_id[2 * v + 1] == leftmost_cid;
}

/*
 * Free all memory associated with the breakpoint graph.
 */
void bg_free(BreakpointGraph *bg)
{
    int sz;

    if (!bg)
        return;
    sz = 2 * (bg->n + 2);
    xfree(bg->grey,     (size_t)sz * sizeof(int));
    xfree(bg->black,    (size_t)sz * sizeof(int));
    xfree(bg->cycle_id, (size_t)sz * sizeof(int));
    xfree(bg, sizeof(BreakpointGraph));
}
