/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   permutation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:11:47 by thirata           #+#    #+#             */
/*   Updated: 2026/04/11 21:26:53 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

/*
 * Permutation convention: 0-indexed, values in {0, 1, ..., n-1}.
 * perm[i] = j means element j is at position i.
 */

/* Fill perm with the identity: perm[i] = i. */
void    identity_permutation(int *perm, int n, int is_natural)
{
    int i;

    for (i = 0; i < n; i++)
        perm[i] = i + is_natural;
}

/* Copy src into dst (both length n). */
void    copy_permutation(int *src, int *dst, int n)
{
    memcpy(dst, src, (size_t)n * sizeof(int));
}

/* Return 1 if perm is the identity, 0 otherwise. */
int     is_identity_permutation(int *perm, int n, int is_natural)
{
    int i;

    if (is_natural != 0 && is_natural != 1)
        return (0);
    for (i = 0; i < n; i++)
        if (perm[i] != i + is_natural)
            return 0;
    return 1;
}

/*
 * Compute the inverse of perm into inv:
 *   inv[perm[i]] = i  for all i.
 * perm and inv must be distinct arrays.
 */
void    inverse_permutation(int *perm, int *inv, int n)
{
    int i;

    for (i = 0; i < n; i++)
        inv[perm[i]] = i;
}

/*
 * Compose permutations p and q into res:
 *   res[i] = p[q[i]]  (q is applied first, then p).
 * res may not alias p or q.
 */
void    compose_permutation(int *p, int *q, int *res, int n)
{
    int i;

    for (i = 0; i < n; i++)
        res[i] = p[q[i]];
}

/*
 * Count breakpoints of an unsigned linear permutation (0-indexed).
 *
 * The extended permutation is: (-1) perm[0] ... perm[n-1] (n).
 * A breakpoint exists between position i-1 and i when perm[i] != perm[i-1] + 1,
 * counting the sentinel boundaries as perm[-1] = -1 and perm[n] = n.
 *
 * Identity permutation has 0 breakpoints; maximum is n+1.
 */
int     count_breakpoints(int *perm, int n)
{
    int i, count;

    count = 0;
    if (perm[0] != 0)
        count++;
    for (i = 0; i < n - 1; i++)
        if (perm[i + 1] != perm[i] + 1)
            count++;
    if (perm[n - 1] != n - 1)
        count++;
    return count;
}

/*
 * Return the 0-indexed position of value val in perm.
 * Returns -1 if not found.
 */
int     find_value(int *perm, int n, int val)
{
    int i;

    for (i = 0; i < n; i++)
        if (perm[i] == val)
            return i;
    return -1;
}

/*
 * Return the smallest 0-indexed i such that perm[i+1] != perm[i]+1.
 * Returns -1 if no such i exists (i.e., perm[0..n-1] is a consecutive run).
 */
int     first_breakpoint_position(int *perm, int n)
{
    int i;

    for (i = 0; i < n - 1; i++)
        if (perm[i + 1] != perm[i] + 1)
            return i;
    return -1;
}
