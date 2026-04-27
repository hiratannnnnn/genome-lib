#ifndef LP_SCORE_H
# define LP_SCORE_H

/*
 * [2021] Miranda et al. — Theorem 3
 * (λ(λ-1)/2 + λ)-approximation for Sorting Signed λ-Permutations
 * by λ-Reversals and by λ-Reversals & λ-Transpositions.
 *
 * score(π, σ) = Δ(Inv(π) + |E^-_even| + |E^+_odd|)
 * Greedy: pick the λ-operation with highest score (≥ 1 guaranteed).
 *
 * mode: 1=signed reversals only, 3=signed reversals & transpositions
 */

# define LP_SCORE_REV 1
# define LP_SCORE_BOTH 3

typedef struct
{
	int		type;
	int		i;
	int		j;
	int		k;
}			LPScoreOp;

LPScoreOp	*lp_score_greedy(int *perm, int n, int lambda, int mode,
				int *op_count);

#endif
