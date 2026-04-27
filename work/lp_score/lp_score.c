#include "lp_score.h"
#include "../foundations/foundations.h"
#include "../../includes/memory.h"

/*
 * [2021] Theorem 3
 * score(π, σ) = (Inv + |E^-_even| + |E^+_odd|)(π) - (same)(π·σ)
 * Lemma 5 guarantees score ≥ 1 always exists.
 *
 * TODO: implement
 */
LPScoreOp *lp_score_greedy(int *perm, int n, int lambda, int mode,
                             int *op_count)
{
    (void)perm; (void)n; (void)lambda; (void)mode;
    *op_count = 0;
    return NULL;
}
