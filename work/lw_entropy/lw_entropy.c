#include "../../includes/memory.h"
#include "../foundations/foundations.h"
#include "lw_entropy.h"
#include <math.h>

/*
 * [2020] Algorithm 2
 * while π ≠ ι:
 *   pick β maximising Δent(π,β) / |β|^alpha
 *   if ratio ≥ 1/2: apply β
 *   else: use Lemma 9 super-short sequence to exchange πi, πj
 *         s.t. |πi| ≥ j and |πj| ≤ i (Lemma 8 pair)
 *
 * TODO: implement
 */
LWEntOp	*lw_ent_greedy(int *perm, int n, int lambda, double alpha, int mode,
		int *op_count)
{
	(void)perm;
	(void)n;
	(void)lambda;
	(void)alpha;
	(void)mode;
	*op_count = 0;
	return (NULL);
}
