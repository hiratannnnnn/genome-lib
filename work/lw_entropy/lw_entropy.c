#include "../../includes/memory.h"
#include "../foundations/foundations.h"
#include "lw_entropy.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*
 * [2020] Algorithm 2 — Length-weighted entropy greedy
 *
 * ent(π) = Σ ||πi| - (i+1)|   (total positional displacement)
 * score(β) = Δent(π,β) / |β|^alpha
 *
 * Main step: pick β maximising score. Apply if score ≥ 0.5.
 * Fallback (Lemma 9): if no β achieves ≥ 0.5, find a Lemma 8 pair
 *   (positions a < b with |perm[a]| ≥ b+1 and |perm[b]| ≤ a+1)
 *   and exchange them via ≤2*(b-a) adjacent size-2 reversals.
 */

/* find Lemma 8 pair: 0-indexed positions a < b
 * where |perm[a]| >= b+1 and |perm[b]| <= a+1 */
static int	find_lem8_pair(int *perm, int n, int *out_a, int *out_b)
{
	for (int a = 0; a < n - 1; a++)
	{
		for (int b = a + 1; b < n; b++)
		{
			if (abs(perm[a]) >= b + 1 && abs(perm[b]) <= a + 1)
			{
				*out_a = a;
				*out_b = b;
				return (1);
			}
		}
	}
	return (0);
}

/* exchange perm[a] and perm[b] via adjacent size-2 reversals
 * (bubble perm[b] left to a, then perm[a] right to b) */
static int	lem9_exchange(int *perm, int n, int a, int b, LWEntOp *out,
		int lambda)
{
	int	cnt;

	cnt = 0;
	if (2 > lambda)
		return (0);
	/* bubble perm[b] to position a */
	for (int pos = b - 1; pos >= a; pos--)
	{
		apply_rev(perm, n, pos, pos + 1);
		out[cnt].type = LW_ENT_MODE_REV;
		out[cnt].i = pos;
		out[cnt].j = pos + 1;
		out[cnt].k = -1;
		cnt++;
	}
	/* bubble original perm[a] (now at a+1) to position b */
	for (int pos = a + 1; pos < b; pos++)
	{
		apply_rev(perm, n, pos, pos + 1);
		out[cnt].type = LW_ENT_MODE_REV;
		out[cnt].i = pos;
		out[cnt].j = pos + 1;
		out[cnt].k = -1;
		cnt++;
	}
	return (cnt);
}

LWEntOp	*lw_ent_greedy(int *perm, int n, int lambda, double alpha, int mode,
		int *op_count)
{
	int		cap;
	LWEntOp	*ops;
	int		cnt;
	int		*cur;
	int		*tmp;
	int		ent_cur;
	double	best_score;
	int		best_i;
	int		best_j;
	int		best_k;
	int		best_type;
	int		dent;
	double	score;
	int		la8_a;
	int		la8_b;
	LWEntOp	local[64];
	int		napplied;

	cap = n * n * 4 + 4;
	ops = xmalloc(sizeof(LWEntOp) * cap);
	cnt = 0;
	cur = xmalloc(sizeof(int) * n);
	tmp = xmalloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	while ((ent_cur = ent_perm(cur, n)) > 0)
	{
		best_score = 0.0;
		best_i = -1;
		best_j = -1;
		best_k = -1;
		best_type = LW_ENT_MODE_REV;
		if (mode == LW_ENT_MODE_REV || mode == LW_ENT_MODE_BOTH)
		{
			for (int i = 0; i < n; i++)
			{
				for (int j = i + 1; j < n && j - i + 1 <= lambda; j++)
				{
					memcpy(tmp, cur, sizeof(int) * n);
					apply_rev(tmp, n, i, j);
					if (!is_lam_perm(tmp, n, lambda))
						continue ;
					dent = ent_cur - ent_perm(tmp, n);
					if (dent <= 0)
						continue ;
					score = (double)dent / pow((double)(j - i + 1), alpha);
					if (score > best_score)
					{
						best_score = score;
						best_i = i;
						best_j = j;
						best_type = LW_ENT_MODE_REV;
					}
				}
			}
		}
		if (mode == LW_ENT_MODE_TPOS || mode == LW_ENT_MODE_BOTH)
		{
			for (int i = 0; i < n; i++)
			{
				for (int j = i + 1; j < n && j - i < lambda; j++)
				{
					for (int k = j + 1; k <= n && k - i <= lambda; k++)
					{
						memcpy(tmp, cur, sizeof(int) * n);
						apply_tpos(tmp, n, i, j, k);
						if (!is_lam_perm(tmp, n, lambda))
							continue ;
						dent = ent_cur - ent_perm(tmp, n);
						if (dent <= 0)
							continue ;
						score = (double)dent / pow((double)(k - i), alpha);
						if (score > best_score)
						{
							best_score = score;
							best_i = i;
							best_j = j;
							best_k = k;
							best_type = LW_ENT_MODE_TPOS;
						}
					}
				}
			}
		}
		/* apply greedy op if score >= 0.5, else Lemma 9 fallback */
		if (best_i >= 0 && best_score >= 0.5)
		{
			if (cnt >= cap - 1)
			{
				cap *= 2;
				ops = xrealloc(ops, sizeof(LWEntOp) * (cap / 2), sizeof(LWEntOp)
						* cap);
			}
			if (best_type == LW_ENT_MODE_REV)
			{
				apply_rev(cur, n, best_i, best_j);
				ops[cnt].type = LW_ENT_MODE_REV;
				ops[cnt].i = best_i;
				ops[cnt].j = best_j;
				ops[cnt].k = -1;
			}
			else
			{
				apply_tpos(cur, n, best_i, best_j, best_k);
				ops[cnt].type = LW_ENT_MODE_TPOS;
				ops[cnt].i = best_i;
				ops[cnt].j = best_j;
				ops[cnt].k = best_k;
			}
			cnt++;
		}
		else
		{
			/* Lemma 9: find Lemma 8 pair and exchange via adjacent swaps */
			if (!find_lem8_pair(cur, n, &la8_a, &la8_b))
				break ;
			napplied = lem9_exchange(cur, n, la8_a, la8_b, local, lambda);
			if (napplied == 0)
				break ;
			if (cnt + napplied >= cap)
			{
				cap = cap * 2 + napplied;
				ops = xrealloc(ops, sizeof(LWEntOp) * (cap / 2 - napplied),
						sizeof(LWEntOp) * cap);
			}
			for (int k2 = 0; k2 < napplied; k2++)
				ops[cnt++] = local[k2];
		}
	}
	xfree(tmp, sizeof(int) * n);
	xfree(cur, sizeof(int) * n);
	*op_count = cnt;
	return (ops);
}
