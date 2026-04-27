#include "../../includes/memory.h"
#include "../foundations/foundations.h"
#include "lp_inversions.h"
#include <string.h>

/*
 * [2021] Theorem 2
 * At each step pick the λ-operation σ such that:
 *   (a) π · σ is a λ-permutation
 *   (b) Inv(π · σ) is minimised
 * Worst case: Inv decreases by 1 per step (Lemma 3 guarantees progress).
 * Approximation factor: O(λ²) via Lemma 1.
 *
 * TODO: implement
 */
LPOp	*lp_inv_greedy(int *perm, int n, int lambda, int mode, int *op_count)
{
	(void)perm;
	(void)n;
	(void)lambda;
	(void)mode;
	*op_count = 0;
	return (NULL);
}
