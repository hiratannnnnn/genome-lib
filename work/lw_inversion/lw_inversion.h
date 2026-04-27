#ifndef LW_INVERSION_H
# define LW_INVERSION_H

/*
 * [2020] Algorithm 1 — Inversion-based greedy
 * Sorts unsigned permutations with length-weighted λ-operations.
 *
 * Cost: f(β) = |β|^alpha
 * Greedy: pick the λ-operation β that maximises ΔInv(π,β) / f(β).
 *
 * Approximation factors (alpha = 1):
 *   SbR, SbRT : (λ-1)-approx        [Theorem 1]
 *   SbT        : (λ/2)-approx        [Theorem 1]
 *   λ=3, SbR,SbRT : 2-approx         [Section 7]
 *   λ=3, SbT      : 4/3-approx       [Lemma 15]
 *   alpha ≥ 2  : 2-approx            [Theorem 8]
 *   alpha ≥ 3  : exact               [Theorem 9]
 *
 * mode: 1=reversals only (SbR), 2=transpositions only (SbT), 3=both (SbRT)
 */

# define LW_MODE_REV   1
# define LW_MODE_TRANS 2
# define LW_MODE_BOTH  3

typedef struct { int type; int i; int j; int k; } LWOp;

LWOp *lw_inversion_greedy(int *perm, int n, int lambda, double alpha,
                            int mode, int *op_count);

#endif
