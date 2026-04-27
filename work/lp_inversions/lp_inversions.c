#include "../../includes/memory.h"
#include "../foundations/foundations.h"
#include "lp_inversions.h"
#include <string.h>

/*
 * [2021] Theorem 2 — O(λ²)-approx for unsigned λ-permutations.
 *
 * At each step, enumerate every valid λ-operation σ such that
 * π·σ is still a λ-permutation, and pick the one that minimises
 * Inv(π·σ) (equivalently maximises delta_inv).  Lemma 3 guarantees
 * that at least one 2-op always reduces Inv by 1, so the loop
 * terminates in at most Inv(π) iterations.
 *
 * Time per step: O(λ²) candidates × O(λ²) delta eval = O(λ⁴).
 * Total: O(n² λ⁴)  (Inv ≤ n(n-1)/2 and each step reduces it by ≥1).
 */

/* try one perm copy and return -delta (lower = better) */
static int	try_rev(int *perm, int n, int lambda, int i, int j, int *best_i,
		int *best_j, int *best_delta)
{
	int	*tmp;
	int	delta;
	int	result;

	if (j - i + 1 > lambda)
		return (0);
	tmp = xmalloc(sizeof(int) * n);
	memcpy(tmp, perm, sizeof(int) * n);
	apply_rev(tmp, n, i, j);
	result = is_lam_perm(tmp, n, lambda);
	if (result)
	{
		delta = delta_inv_rev(perm, n, i, j);
		if (delta > *best_delta)
		{
			*best_delta = delta;
			*best_i = i;
			*best_j = j;
		}
	}
	xfree(tmp, sizeof(int) * n);
	return (result);
}

static int	try_tpos(int *perm, int n, int lambda, int i, int j, int k,
		int *best_i, int *best_j, int *best_k, int *best_delta, int *best_type)
{
	int	*tmp;
	int	delta;
	int	result;

	if (k - i > lambda || i >= j || j >= k)
		return (0);
	tmp = xmalloc(sizeof(int) * n);
	memcpy(tmp, perm, sizeof(int) * n);
	apply_tpos(tmp, n, i, j, k);
	result = is_lam_perm(tmp, n, lambda);
	if (result)
	{
		delta = delta_inv_tpos(perm, n, i, j, k);
		if (delta > *best_delta)
		{
			*best_delta = delta;
			*best_i = i;
			*best_j = j;
			*best_k = k;
			*best_type = LP_MODE_TPOS;
		}
	}
	xfree(tmp, sizeof(int) * n);
	return (result);
}

LPOp	*lp_inv_greedy(int *perm, int n, int lambda, int mode, int *op_count)
{
	int		capacity;
	LPOp	*ops;
	int		cnt;
	int		*cur;
	int		best_i;
	int		best_j;
	int		best_k;
	int		best_delta;
	int		best_type;
	int		pos;

	capacity = n * n;
	ops = xmalloc(sizeof(LPOp) * capacity);
	cnt = 0;
	cur = xmalloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	pos = find_soop(cur, n);
	while (pos >= 0)
	{
		best_i = -1;
		best_j = -1;
		best_k = -1;
		best_delta = 0;
		best_type = LP_MODE_REV;
		if (mode == LP_MODE_REV || mode == LP_MODE_BOTH)
		{
			for (int i = 0; i < n; i++)
				for (int j = i + 1; j < n && j - i + 1 <= lambda; j++)
					try_rev(cur, n, lambda, i, j, &best_i, &best_j,
						&best_delta);
		}
		if (mode == LP_MODE_TPOS || mode == LP_MODE_BOTH)
		{
			for (int i = 0; i < n; i++)
				for (int j = i + 1; j < n && j - i < lambda; j++)
					for (int k = j + 1; k <= n && k - i <= lambda; k++)
						try_tpos(cur, n, lambda, i, j, k, &best_i, &best_j,
							&best_k, &best_delta, &best_type);
		}
		if (best_i < 0)
			break ;
		if (best_type == LP_MODE_REV)
		{
			apply_rev(cur, n, best_i, best_j);
			if (cnt >= capacity)
			{
				capacity *= 2;
				ops = xrealloc(ops, sizeof(LPOp) * (capacity / 2), sizeof(LPOp)
						* capacity);
			}
			ops[cnt].type = LP_MODE_REV;
			ops[cnt].i = best_i;
			ops[cnt].j = best_j;
			ops[cnt].k = -1;
			cnt++;
		}
		else
		{
			apply_tpos(cur, n, best_i, best_j, best_k);
			if (cnt >= capacity)
			{
				capacity *= 2;
				ops = xrealloc(ops, sizeof(LPOp) * (capacity / 2), sizeof(LPOp)
						* capacity);
			}
			ops[cnt].type = LP_MODE_TPOS;
			ops[cnt].i = best_i;
			ops[cnt].j = best_j;
			ops[cnt].k = best_k;
			cnt++;
		}
		pos = find_soop(cur, n);
	}
	xfree(cur, sizeof(int) * n);
	*op_count = cnt;
	return (ops);
}
