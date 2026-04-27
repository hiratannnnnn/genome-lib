#ifndef LP_INVERSIONS_H
# define LP_INVERSIONS_H

/*
 * [2021] Miranda et al. — Theorem 2
 * O(λ²)-approximation for Sorting Unsigned λ-Permutations
 * by λ-Reversals, λ-Transpositions, and both.
 *
 * Greedy: at each step, choose the λ-operation that minimizes
 * the inversion count while keeping the result a λ-permutation.
 *
 * mode: 1=reversals only, 2=transpositions only, 3=both
 */

# define LP_MODE_REV   1
# define LP_MODE_TRANS 2
# define LP_MODE_BOTH  3

typedef struct { int type; int i; int j; int k; } LPOp;

LPOp *lp_inversions_greedy(int *perm, int n, int lambda, int mode,
                             int *op_count);

#endif
