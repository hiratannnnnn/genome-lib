#include "../../includes/memory.h"
#include "../foundations/foundations.h"
#include "lp_score.h"
#include <string.h>

/*
 * [2021] Theorem 3 — (λ(λ-1)/2 + λ)-approx for signed λ-permutations.
 *
 * score(π, σ) = (Inv(π) + |E^-_even|(π) + |E^+_odd|(π))
 *             - (Inv(π·σ) + |E^-_even|(π·σ) + |E^+_odd|(π·σ))
 *
 * At each step pick the λ-operation σ (signed reversal, and optionally
 * transposition) such that π·σ is a λ-permutation and score(π,σ) is
 * maximised.  Lemma 5 guarantees score ≥ 1 always exists.
 */

static int	potential(int *perm, int n)
{
	return (cnt_inv(perm, n) + cnt_Eminus(perm, n) + cnt_Eplus(perm, n));
}

static void	try_srev(int *perm, int n, int lambda, int i, int j, int *best_i,
		int *best_j, int *best_k, int *best_score, int *best_type)
{
	int	*tmp;
	int	score;

	if (j - i + 1 > lambda)
		return ;
	tmp = xmalloc(sizeof(int) * n);
	memcpy(tmp, perm, sizeof(int) * n);
	apply_srev(tmp, n, i, j);
	if (is_lam_perm(tmp, n, lambda))
	{
		score = potential(perm, n) - potential(tmp, n);
		if (score > *best_score)
		{
			*best_score = score;
			*best_i = i;
			*best_j = j;
			*best_k = -1;
			*best_type = LP_SCORE_REV;
		}
	}
	xfree(tmp, sizeof(int) * n);
}

static void	try_score_tpos(int *perm, int n, int lambda, int i, int j, int k,
		int *best_i, int *best_j, int *best_k, int *best_score, int *best_type)
{
	int	*tmp;
	int	score;

	if (k - i > lambda || i >= j || j >= k)
		return ;
	tmp = xmalloc(sizeof(int) * n);
	memcpy(tmp, perm, sizeof(int) * n);
	apply_tpos(tmp, n, i, j, k);
	if (is_lam_perm(tmp, n, lambda))
	{
		score = potential(perm, n) - potential(tmp, n);
		if (score > *best_score)
		{
			*best_score = score;
			*best_i = i;
			*best_j = j;
			*best_k = k;
			*best_type = LP_SCORE_BOTH;
		}
	}
	xfree(tmp, sizeof(int) * n);
}

LPScoreOp	*lp_score_greedy(int *perm, int n, int lambda, int mode,
		int *op_count)
{
	int			capacity;
	LPScoreOp	*ops;
	int			cnt;
	int			*cur;
	int			best_i;
	int			best_j;
	int			best_k;
	int			best_score;
	int			best_type;

	capacity = n * n;
	ops = xmalloc(sizeof(LPScoreOp) * capacity);
	cnt = 0;
	cur = xmalloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	while (potential(cur, n) > 0)
	{
		best_i = -1;
		best_j = -1;
		best_k = -1;
		best_score = 0;
		best_type = LP_SCORE_REV;
		for (int i = 0; i < n; i++)
			for (int j = i; j < n && j - i + 1 <= lambda; j++)
				try_srev(cur, n, lambda, i, j, &best_i, &best_j, &best_k,
					&best_score, &best_type);
		if (mode == LP_SCORE_BOTH)
			for (int i = 0; i < n; i++)
				for (int j = i + 1; j < n && j - i < lambda; j++)
					for (int k = j + 1; k <= n && k - i <= lambda; k++)
						try_score_tpos(cur, n, lambda, i, j, k, &best_i,
							&best_j, &best_k, &best_score, &best_type);
		if (best_i < 0)
			break ;
		if (best_type == LP_SCORE_REV)
			apply_srev(cur, n, best_i, best_j);
		else
			apply_tpos(cur, n, best_i, best_j, best_k);
		if (cnt >= capacity)
		{
			capacity *= 2;
			ops = xrealloc(ops, sizeof(LPScoreOp) * (capacity / 2),
					sizeof(LPScoreOp) * capacity);
		}
		ops[cnt].type = best_type;
		ops[cnt].i = best_i;
		ops[cnt].j = best_j;
		ops[cnt].k = best_k;
		cnt++;
	}
	xfree(cur, sizeof(int) * n);
	*op_count = cnt;
	return (ops);
}
