/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   approx_sbpbi.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:10:37 by thirata           #+#    #+#             */
/*   Updated: 2026/04/10 08:59:37 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"
#include "approx_sbpbi.h"

/*
 * Index translation between paper notation and code:
 *
 *   Paper: π is 1-indexed, values in {1,...,n}.
 *   Code:  perm is 0-indexed, values in {0,...,n-1}.
 *   Relation: paper πᵢ = perm[i-1]+1.
 *
 *   Paper β(1, paper_i, paper_j, paper_k) is implemented as:
 *     prefix_block_interchange(arr, n, paper_i-1, paper_j, paper_k-1)
 *
 *   Given 0-indexed search results (i_code, j_code, k_code):
 *     paper_i = i_code+1, paper_j = j_code+1, paper_k = k_code+1
 *   => prefix_block_interchange(arr, n, i_code, j_code+1, k_code)
 */

/*
 * Build the position array: pos[v] = position of value v in perm.
 * Caller must free with xfree(pos, n * sizeof(int)).
 */
static int *build_pos(int *perm, int n)
{
    int *pos;
    int  i;

    pos = (int *)xmalloc((size_t)n * sizeof(int));
    for (i = 0; i < n; i++)
        pos[perm[i]] = i;
    return pos;
}

/*
 * Case A: find (i_code, k_code) such that
 *   i_code <= j_code < k_code,
 *   perm[k_code] == perm[i_code]+1,
 *   the black arc at pbar-value perm[i_code]+1 is in the leftmost cycle.
 *
 * Returns 1 if found, 0 otherwise.
 */
static int find_case_a(int *perm, int n, int j_code, BreakpointGraph *bg,
                        int *out_i, int *out_k)
{
    int *pos;
    int  i, k;

    pos = build_pos(perm, n);
    for (i = 0; i <= j_code; i++)
    {
        if (perm[i] + 1 >= n)           /* value perm[i]+1 must exist in perm */
            continue;
        k = pos[perm[i] + 1];
        if (k > j_code && bg_black_arc_in_leftmost(bg, perm[i] + 1))
        {
            *out_i = i;
            *out_k = k;
            xfree(pos, (size_t)n * sizeof(int));
            return 1;
        }
    }
    xfree(pos, (size_t)n * sizeof(int));
    return 0;
}

/*
 * Case B: find (i_code, k_code) such that
 *   i_code <= j_code < k_code,
 *   perm[k_code] == perm[i_code]-1.
 *
 * The proof guarantees this always succeeds when Case A fails.
 * Returns 1 if found, 0 otherwise.
 */
static int find_case_b(int *perm, int n, int j_code, int *out_i, int *out_k)
{
    int *pos;
    int  i, k;

    pos = build_pos(perm, n);
    for (i = 0; i <= j_code; i++)
    {
        if (perm[i] - 1 < 0)            /* value perm[i]-1 must exist in perm */
            continue;
        k = pos[perm[i] - 1];
        if (k > j_code)
        {
            *out_i = i;
            *out_k = k;
            xfree(pos, (size_t)n * sizeof(int));
            return 1;
        }
    }
    xfree(pos, (size_t)n * sizeof(int));
    return 0;
}

/*
 * ApproximateSbpbi: 2-approximation for sorting by prefix block-interchanges.
 *
 * perm: permutation (0-indexed, values 0..n-1), sorted in-place.
 * ops:  if non-NULL, records each operation as {j, k, l} (args to
 *       prefix_block_interchange). Caller must provide space for >= n entries.
 * Returns: number of operations applied.
 */
int approx_sbpbi(int *perm, int n, int (*ops)[3])
{
    BreakpointGraph *bg;
    int              num_ops;
    int              op_j, op_k, op_l;
    int              i_code, k_code, j_code, bp;

    num_ops = 0;

    while (!is_identity_permutation(perm, n))
    {
        bg = bg_build(perm, n);
        bg_decompose_cycles(bg);

        if (perm[0] != 0)
        {
            /* j_code = position of value perm[0]-1 */
            j_code = find_value(perm, n, perm[0] - 1);

            if (find_case_a(perm, n, j_code, bg, &i_code, &k_code))
            {
                /* Case A: Lemma 4 cases 1-3 */
                op_j = i_code;
                op_k = j_code + 1;
                op_l = k_code;
            }
            else
            {
                /* Case B: Lemma 4 case 4 (guaranteed to succeed) */
                find_case_b(perm, n, j_code, &i_code, &k_code);
                op_j = i_code;
                op_k = j_code + 1;
                op_l = k_code;
            }
        }
        else
        {
            /* Case C: Theorem 5 — perm[0] == 0 */
            bp     = first_breakpoint_position(perm, n);
            /* perm[0..bp] = 0..bp (consecutive from 0), perm[bp+1] != bp+1 */
            /* find position of value bp+1 = perm[bp]+1 */
            j_code = find_value(perm, n, perm[bp] + 1);
            /* paper β(1, bp+1, bp+1, j_code+1) */
            op_j = bp;
            op_k = bp + 1;
            op_l = j_code;
        }

        print_array_int(perm, n, 0);
        bg_free(bg);

        prefix_block_interchange(perm, n, op_j, op_k, op_l);
        if (ops)
        {
            ops[num_ops][0] = op_j;
            ops[num_ops][1] = op_k;
            ops[num_ops][2] = op_l;
        }
        num_ops++;
    }

    return num_ops;
}
