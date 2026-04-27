#include "lw_phi.h"
#include "../foundations/foundations.h"
#include "../../includes/memory.h"
#include <math.h>

/*
 * [2020] Algorithm 3 — φ-score
 * while π ≠ ι:
 *   pick signed λ-operation β maximising φ(π,β) = Δ(2Inv+|E-even|+|E+odd|)/f(β)
 *   apply β
 *
 * TODO: implement φ-score computation and greedy loop
 */
LWPhiOp *lw_phi_greedy(int *perm, int n, int lambda, double alpha,
                         int mode, int *op_count)
{
    (void)perm; (void)n; (void)lambda; (void)alpha; (void)mode;
    *op_count = 0;
    return NULL;
}

/*
 * ψ-score: λ=3 special case for SbR̄T
 * ψ(π,β) = Δ(2Inv + codd(G(π))) / f(β)
 * codd: # odd connected components of the inversion graph G(π)
 *
 * TODO: build inversion graph G(π), compute codd, greedy loop
 */
LWPhiOp *lw_psi_greedy_short(int *perm, int n, int *op_count)
{
    (void)perm; (void)n;
    *op_count = 0;
    return NULL;
}
