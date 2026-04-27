#ifndef LW_PHI_H
# define LW_PHI_H

/*
 * [2020] Algorithm 3 — φ-score greedy (signed permutations)
 *
 * φ(π,β) = Δ(2·Inv(π) + |E^-_even| + |E^+_odd|) / f(β)
 * Greedy: pick signed λ-operation with highest φ.
 * Lemma 12 guarantees φ ≥ 1 always exists.
 *
 * Approximation factor: λ-approx for SbR̄, SbR̄T  [Theorem 3]
 * alpha ≥ 2: 3-approx  [Theorem 11]
 * alpha ≥ 3: exact     [Theorems 12, 13]
 *
 * Also implements ψ-score for the λ=3 special case:
 * ψ(π,β) = Δ(2·Inv(π) + codd(G(π))) / f(β)
 * Approximation factor: 7/3-approx for SbR̄T (λ=3)  [Theorem 7]
 *
 * mode: 1=signed reversals (SbRbar), 3=signed reversals & transpositions (SbRbarT)
 */

# define LW_PHI_RBAR  1
# define LW_PHI_RBBAR 3

typedef struct { int type; int i; int j; int k; } LWPhiOp;

LWPhiOp *lw_phi_greedy(int *perm, int n, int lambda, double alpha,
                         int mode, int *op_count);

/* ψ-score algorithm: only valid for lambda=3 */
LWPhiOp *lw_psi_greedy_short(int *perm, int n, int *op_count);

#endif
