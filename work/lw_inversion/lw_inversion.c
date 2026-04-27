#include "../../includes/memory.h"
#include "../foundations/foundations.h"
#include "lw_inversion.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*
 * [2020] Algorithm 1 — Length-weighted inversion greedy
 *
 * Score for each candidate op β: ΔInv(π,β) / |β|^alpha
 *   |ρ(i,j)| = j-i+1,  |τ(i,j,k)| = k-i
 *
 * Pick the op with highest score (positive score only).
 * Terminates because each step strictly reduces Inv(π) ≥ 1
 * (Theorem 1 guarantees existence of such an op for λ-perms).
 */

LWOp	*lw_inv_greedy(int *perm, int n, int lambda, double alpha, int mode,
		int *op_count)
{
	int		cap;
	LWOp	*ops;
	int		cnt;
	int		*cur;
	int		*tmp;
	double	best_score;
	int		best_i;
	int		best_j;
	int		best_k;
	int		best_type;
	int		dinv;
	double	score;

	cap = n * n + 4;
	ops = xmalloc(sizeof(LWOp) * cap);
	cnt = 0;
	cur = xmalloc(sizeof(int) * n);
	tmp = xmalloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	while (cnt_inv(cur, n) > 0)
	{
		best_score = 0.0;
		best_i = -1;
		best_j = -1;
		best_k = -1;
		best_type = LW_MODE_REV;
		if (mode == LW_MODE_REV || mode == LW_MODE_BOTH)
		{
			for (int i = 0; i < n; i++)
			{
				for (int j = i + 1; j < n && j - i + 1 <= lambda; j++)
				{
					dinv = delta_inv_rev(cur, n, i, j);
					if (dinv <= 0)
						continue ;
					memcpy(tmp, cur, sizeof(int) * n);
					apply_rev(tmp, n, i, j);
					if (!is_lam_perm(tmp, n, lambda))
						continue ;
					score = (double)dinv / pow((double)(j - i + 1), alpha);
					if (score > best_score)
					{
						best_score = score;
						best_i = i;
						best_j = j;
						best_type = LW_MODE_REV;
					}
				}
			}
		}
		if (mode == LW_MODE_TPOS || mode == LW_MODE_BOTH)
		{
			for (int i = 0; i < n; i++)
			{
				for (int j = i + 1; j < n && j - i < lambda; j++)
				{
					for (int k = j + 1; k <= n && k - i <= lambda; k++)
					{
						dinv = delta_inv_tpos(cur, n, i, j, k);
						if (dinv <= 0)
							continue ;
						memcpy(tmp, cur, sizeof(int) * n);
						apply_tpos(tmp, n, i, j, k);
						if (!is_lam_perm(tmp, n, lambda))
							continue ;
						score = (double)dinv / pow((double)(k - i), alpha);
						if (score > best_score)
						{
							best_score = score;
							best_i = i;
							best_j = j;
							best_k = k;
							best_type = LW_MODE_TPOS;
						}
					}
				}
			}
		}
		if (best_i < 0)
			break ;
		if (cnt >= cap)
		{
			cap *= 2;
			ops = xrealloc(ops, sizeof(LWOp) * (cap / 2), sizeof(LWOp) * cap);
		}
		if (best_type == LW_MODE_REV)
		{
			apply_rev(cur, n, best_i, best_j);
			ops[cnt].type = LW_MODE_REV;
			ops[cnt].i = best_i;
			ops[cnt].j = best_j;
			ops[cnt].k = -1;
		}
		else
		{
			apply_tpos(cur, n, best_i, best_j, best_k);
			ops[cnt].type = LW_MODE_TPOS;
			ops[cnt].i = best_i;
			ops[cnt].j = best_j;
			ops[cnt].k = best_k;
		}
		cnt++;
	}
	xfree(tmp, sizeof(int) * n);
	xfree(cur, sizeof(int) * n);
	*op_count = cnt;
	return (ops);
}
