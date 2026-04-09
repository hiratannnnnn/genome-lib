#include "lib.h"
#include <string.h>

/*
 * Permutation convention: 0-indexed, values in {0, 1, ..., n-1}.
 * perm[i] = j means element j is at position i.
 */

/* Fill perm with the identity: perm[i] = i. */
void    identity_permutation(int *perm, int n)
{
    int i;

    for (i = 0; i < n; i++)
        perm[i] = i;
}

/* Copy src into dst (both length n). */
void    copy_permutation(int *src, int *dst, int n)
{
    memcpy(dst, src, (size_t)n * sizeof(int));
}

/* Return 1 if perm is the identity, 0 otherwise. */
int     is_identity_permutation(int *perm, int n)
{
    int i;

    for (i = 0; i < n; i++)
        if (perm[i] != i)
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
 * Block interchange: swap arr[i..j] with arr[k..l].
 *
 * Precondition: 0 <= i <= j < k <= l < n.
 * The segment arr[j+1..k-1] (the "middle") shifts but is otherwise untouched.
 *
 * Before: ... [i..j] [j+1..k-1] [k..l] ...
 * After:  ... [k..l] [j+1..k-1] [i..j] ...
 */
void    block_interchange(int *arr, int n, int i, int j, int k, int l)
{
    int len1, len2, mid, total;
    int *tmp;

    if (!arr || i < 0 || i > j || j >= k || k > l || l >= n)
        return;

    len1  = j - i + 1;
    len2  = l - k + 1;
    mid   = k - j - 1;
    total = l - i + 1;

    tmp = (int *)xmalloc((size_t)total * sizeof(int));
    if (!tmp)
        return;

    /* New layout starting at position i: [block2][middle][block1] */
    memcpy(tmp,              arr + k,     (size_t)len2 * sizeof(int));
    memcpy(tmp + len2,       arr + j + 1, (size_t)mid  * sizeof(int));
    memcpy(tmp + len2 + mid, arr + i,     (size_t)len1 * sizeof(int));
    memcpy(arr + i, tmp, (size_t)total * sizeof(int));

    xfree(tmp, (size_t)total * sizeof(int));
}

/*
 * Prefix block interchange: swap arr[0..j] with arr[k..l].
 *
 * This is block_interchange with i = 0.
 * Precondition: 0 <= j < k <= l < n.
 */
void    prefix_block_interchange(int *arr, int n, int j, int k, int l)
{
    block_interchange(arr, n, 0, j, k, l);
}
