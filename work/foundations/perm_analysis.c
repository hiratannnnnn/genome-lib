#include "foundations.h"
#include <stdlib.h>

/* ── Inversions ─────────────────────────────────────────────────── */

int count_inversions(int *perm, int n)
{
    int inv = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (abs(perm[i]) > abs(perm[j]))
                inv++;
    return inv;
}

/* change in Inv caused by unsigned reversal ρ(i,j) (0-indexed) */
int delta_inv_reversal(int *perm, int n, int i, int j)
{
    (void)n;
    int delta = 0;
    for (int a = i; a <= j; a++)
        for (int b = a + 1; b <= j; b++)
            if (abs(perm[a]) > abs(perm[b]))
                delta++;   /* inversion inside segment — survives reversal */
    /* total inversions in segment before reversal */
    int total = 0;
    for (int a = i; a < j; a++)
        for (int b = a + 1; b <= j; b++)
            if (abs(perm[a]) > abs(perm[b]))
                total++;
    int seg_len = j - i + 1;
    int max_inv  = seg_len * (seg_len - 1) / 2;
    /* after reversal, inversions inside become (max_inv - total) */
    return total - (max_inv - total);
}

int delta_inv_transposition(int *perm, int n, int i, int j, int k)
{
    (void)n;
    int removed = 0;
    for (int a = i; a < j; a++)
        for (int b = j; b < k; b++)
            if (abs(perm[a]) > abs(perm[b]))
                removed++;
    int added = 0;
    for (int a = j; a < k; a++)
        for (int b = i; b < j; b++)
            if (abs(perm[a]) > abs(perm[b]))
                added++;
    return removed - added;
}

/* ── Entropy ─────────────────────────────────────────────────────── */

/* ent(πi) = ||perm[i]| - (i+1)|  (position is 1-indexed in paper) */
int entropy_element(int val, int pos_0indexed)
{
    int diff = abs(val) - (pos_0indexed + 1);
    return diff < 0 ? -diff : diff;
}

int entropy_perm(int *perm, int n)
{
    int e = 0;
    for (int i = 0; i < n; i++)
        e += entropy_element(perm[i], i);
    return e;
}

/* E^-_even: negative elements with even entropy */
int count_E_minus_even(int *perm, int n)
{
    int cnt = 0;
    for (int i = 0; i < n; i++)
        if (perm[i] < 0 && entropy_element(perm[i], i) % 2 == 0)
            cnt++;
    return cnt;
}

/* E^+_odd: positive elements with odd entropy */
int count_E_plus_odd(int *perm, int n)
{
    int cnt = 0;
    for (int i = 0; i < n; i++)
        if (perm[i] > 0 && entropy_element(perm[i], i) % 2 == 1)
            cnt++;
    return cnt;
}

/* ── Breakpoints ─────────────────────────────────────────────────── */

/* unsigned reversal breakpoint: |perm[i+1] - perm[i]| != 1
 * includes sentinel: perm[-1]=0, perm[n]=n+1 (1-indexed values) */
int count_breakpoints_unsigned_rev(int *perm, int n)
{
    int bp = 0;
    int prev = 0;
    for (int i = 0; i < n; i++)
    {
        int diff = abs(perm[i]) - prev;
        if (diff != 1 && diff != -1)
            bp++;
        prev = abs(perm[i]);
    }
    if (abs(perm[n - 1]) != n)
        bp++;
    return bp;
}

/* transposition / signed reversal breakpoint: perm[i+1] - perm[i] != 1 */
int count_breakpoints_transposition(int *perm, int n)
{
    int bp = 0;
    if (perm[0] != 1)
        bp++;
    for (int i = 0; i + 1 < n; i++)
        if (perm[i + 1] - perm[i] != 1)
            bp++;
    if (perm[n - 1] != n)
        bp++;
    return bp;
}

/* ── Lambda-permutation ──────────────────────────────────────────── */

/* λ-permutation: ||perm[i]| - (i+1)| < lambda for all i (0-indexed) */
int is_lambda_perm(int *perm, int n, int lambda)
{
    for (int i = 0; i < n; i++)
    {
        int diff = abs(perm[i]) - (i + 1);
        if (diff < 0) diff = -diff;
        if (diff >= lambda)
            return 0;
    }
    return 1;
}

/* smallest element out-of-place (returns 0-indexed position, or -1 if sorted) */
int find_smallest_out_of_place(int *perm, int n)
{
    for (int v = 1; v <= n; v++)
    {
        for (int i = 0; i < n; i++)
        {
            if (abs(perm[i]) == v && i + 1 != v)
                return i;
        }
    }
    return -1;
}

/* ── Strips ──────────────────────────────────────────────────────── */

Strip *find_strips(int *perm, int n, int is_signed, int *strip_count)
{
    Strip *strips = xmalloc(sizeof(Strip) * n);
    int   cnt     = 0;
    int   start   = 0;

    for (int i = 0; i < n; )
    {
        strips[cnt].start = i;
        if (is_signed)
        {
            int is_inc = (perm[i] > 0);
            int j = i + 1;
            while (j < n && (perm[j] > 0) == is_inc
                   && perm[j] - perm[j - 1] == 1)
                j++;
            strips[cnt].end          = j - 1;
            strips[cnt].is_increasing = is_inc;
        }
        else
        {
            int j = i + 1;
            int asc = (j < n && perm[j] - perm[j - 1] == 1);
            while (j < n && ((asc && perm[j] - perm[j - 1] == 1)
                              || (!asc && perm[j - 1] - perm[j] == 1)))
                j++;
            strips[cnt].end           = j - 1;
            strips[cnt].is_increasing = asc || (strips[cnt].end == strips[cnt].start);
        }
        i = strips[cnt].end + 1;
        cnt++;
    }
    (void)start;
    *strip_count = cnt;
    return strips;
}
