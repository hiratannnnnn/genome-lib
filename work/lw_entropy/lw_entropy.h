#ifndef LW_ENTROPY_H
# define LW_ENTROPY_H

/*
 * [2020] Algorithm 2 — Entropy-based greedy
 * Sorts unsigned permutations with length-weighted λ-operations.
 *
 * Greedy: pick β maximising Δent(π,β) / f(β).
 * When no β achieves ratio ≥ 1/2, fall back to Lemma 9's
 * super-short sequence (exchange πi and πj via ≤4(j-i) reversals).
 *
 * Approximation factor: λ-approx for SbR, SbT, SbRT  [Theorem 2]
 *
 * mode: 1=reversals, 2=transpositions, 3=both
 */

# define LW_ENT_MODE_REV 1
# define LW_ENT_MODE_TPOS 2
# define LW_ENT_MODE_BOTH 3

typedef struct
{
	int	type;
	int	i;
	int	j;
	int	k;
}		LWEntOp;

LWEntOp	*lw_ent_greedy(int *perm, int n, int lambda, double alpha, int mode,
			int *op_count);

#endif
