#include "../../includes/memory.h"
#include "../foundations/foundations.h"
#include "lw_phi.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*
 * [2020] Algorithm 3 — Length-weighted φ-score greedy (signed)
 *
 * Φ(π) = 2·Inv(π) + |E⁻_even| + |E⁺_odd|
 * φ(π,β) = ΔΦ(π,β) / |β|^alpha  (maximise)
 *
 * Uses signed reversals (apply_srev). Mode LW_PHI_RBBAR also allows
 * unsigned transpositions (apply_tpos) for additional progress.
 *
 * Lemma 12 guarantees φ ≥ 1 exists at each step, so Φ decreases
 * by ≥ 1 per iteration and the loop terminates in ≤ Φ(π) steps.
 */

static int	phi_potential(int *perm, int n)
{
	return (2 * cnt_inv(perm, n) + cnt_Eminus(perm, n) + cnt_Eplus(perm, n));
}

LWPhiOp	*lw_phi_greedy(int *perm, int n, int lambda, double alpha, int mode,
		int *op_count)
{
	int		cap;
	LWPhiOp	*ops;
	int		cnt;
	int		*cur;
	int		*tmp;
	int		pot_cur;
	double	best_score;
	int		best_i;
	int		best_j;
	int		best_k;
	int		best_type;
	int		dphi;
	double	score;

	cap = n * n + 4;
	ops = xmalloc(sizeof(LWPhiOp) * cap);
	cnt = 0;
	cur = xmalloc(sizeof(int) * n);
	tmp = xmalloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	while ((pot_cur = phi_potential(cur, n)) > 0)
	{
		best_score = 0.0;
		best_i = -1;
		best_j = -1;
		best_k = -1;
		best_type = LW_PHI_RBAR;
		/* try signed reversals */
		for (int i = 0; i < n; i++)
		{
			for (int j = i; j < n && j - i + 1 <= lambda; j++)
			{
				memcpy(tmp, cur, sizeof(int) * n);
				apply_srev(tmp, n, i, j);
				if (!is_lam_perm(tmp, n, lambda))
					continue ;
				dphi = pot_cur - phi_potential(tmp, n);
				if (dphi <= 0)
					continue ;
				score = (double)dphi / pow((double)(j - i + 1), alpha);
				if (score > best_score)
				{
					best_score = score;
					best_i = i;
					best_j = j;
					best_type = LW_PHI_RBAR;
				}
			}
		}
		/* try transpositions (mode BOTH only) */
		if (mode == LW_PHI_RBBAR)
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
						dphi = pot_cur - phi_potential(tmp, n);
						if (dphi <= 0)
							continue ;
						score = (double)dphi / pow((double)(k - i), alpha);
						if (score > best_score)
						{
							best_score = score;
							best_i = i;
							best_j = j;
							best_k = k;
							best_type = LW_PHI_RBBAR;
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
			ops = xrealloc(ops, sizeof(LWPhiOp) * (cap / 2), sizeof(LWPhiOp)
					* cap);
		}
		if (best_type == LW_PHI_RBAR)
		{
			apply_srev(cur, n, best_i, best_j);
			ops[cnt].type = LW_PHI_RBAR;
			ops[cnt].i = best_i;
			ops[cnt].j = best_j;
			ops[cnt].k = -1;
		}
		else
		{
			apply_tpos(cur, n, best_i, best_j, best_k);
			ops[cnt].type = LW_PHI_RBBAR;
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

/*
 * ψ-score: λ=3 special case for signed rev + tpos (SbR̄T).
 * ψ(π,β) = Δ(2·Inv + codd(G(|π|))) / f(β)  where f(β) = |β|^1.
 *
 * Loop guard: phi_potential > 0 (equivalent to "not sorted"), since
 * ψ alone doesn't reach 0 for sign-only errors.  When no op improves
 * ψ (sign-only residual), fall back to the best Δφ/f(β) candidate.
 */
LWPhiOp	*lw_psi_greedy_short(int *perm, int n, int *op_count)
{
	const int		lambda = 3;
	const double	alpha = 1.0;
	int				cap;
	LWPhiOp			*ops;
	int				cnt;
	int				*cur;
	int				*tmp;
	int				psi_cur;
	int				phi_cur;
	double			best_psi;
	double			best_phi;
	int				best_i;
	int				best_j;
	int				best_k;
	int				best_type;
	int				fallback_i;
	int				fallback_j;
	int				fallback_k;
	int				fallback_type;
	double			sc;
	int				dpsi;
	int				dphi;
	int				dpsi;
	int				dphi;

	cap = n * n + 4;
	ops = xmalloc(sizeof(LWPhiOp) * cap);
	cnt = 0;
	cur = xmalloc(sizeof(int) * n);
	tmp = xmalloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	while ((phi_cur = phi_potential(cur, n)) > 0)
	{
		psi_cur = 2 * cnt_inv(cur, n) + codd_perm(cur, n);
		best_psi = 0.0;
		best_phi = 0.0;
		best_i = -1;
		best_j = -1;
		best_k = -1;
		best_type = LW_PHI_RBAR;
		fallback_i = -1;
		fallback_j = -1;
		fallback_k = -1;
		fallback_type = LW_PHI_RBAR;
		for (int i = 0; i < n; i++)
		{
			for (int j = i; j < n && j - i + 1 <= lambda; j++)
			{
				memcpy(tmp, cur, sizeof(int) * n);
				apply_srev(tmp, n, i, j);
				if (!is_lam_perm(tmp, n, lambda))
					continue ;
				dpsi = psi_cur - (2 * cnt_inv(tmp, n) + codd_perm(tmp, n));
				dphi = phi_cur - phi_potential(tmp, n);
				if (dpsi > 0)
				{
					sc = (double)dpsi / pow((double)(j - i + 1), alpha);
					if (sc > best_psi)
					{
						best_psi = sc;
						best_i = i;
						best_j = j;
						best_k = -1;
						best_type = LW_PHI_RBAR;
					}
				}
				if (dphi > 0)
				{
					sc = (double)dphi / pow((double)(j - i + 1), alpha);
					if (sc > best_phi)
					{
						best_phi = sc;
						fallback_i = i;
						fallback_j = j;
						fallback_k = -1;
						fallback_type = LW_PHI_RBAR;
					}
				}
			}
			for (int j = i + 1; j < n && j - i < lambda; j++)
			{
				for (int k = j + 1; k <= n && k - i <= lambda; k++)
				{
					memcpy(tmp, cur, sizeof(int) * n);
					apply_tpos(tmp, n, i, j, k);
					if (!is_lam_perm(tmp, n, lambda))
						continue ;
					dpsi = psi_cur - (2 * cnt_inv(tmp, n) + codd_perm(tmp, n));
					dphi = phi_cur - phi_potential(tmp, n);
					if (dpsi > 0)
					{
						sc = (double)dpsi / pow((double)(k - i), alpha);
						if (sc > best_psi)
						{
							best_psi = sc;
							best_i = i;
							best_j = j;
							best_k = k;
							best_type = LW_PHI_RBBAR;
						}
					}
					if (dphi > 0)
					{
						sc = (double)dphi / pow((double)(k - i), alpha);
						if (sc > best_phi)
						{
							best_phi = sc;
							fallback_i = i;
							fallback_j = j;
							fallback_k = k;
							fallback_type = LW_PHI_RBBAR;
						}
					}
				}
			}
		}
		/* prefer ψ-best; fall back to φ-best when no ψ-progress (sign residual) */
		if (best_i < 0)
		{
			if (fallback_i >= 0)
			{
				best_i = fallback_i;
				best_j = fallback_j;
				best_k = fallback_k;
				best_type = fallback_type;
			}
			else
				break ;
		}
		if (cnt >= cap)
		{
			cap *= 2;
			ops = xrealloc(ops, sizeof(LWPhiOp) * (cap / 2), sizeof(LWPhiOp)
					* cap);
		}
		if (best_type == LW_PHI_RBAR)
		{
			apply_srev(cur, n, best_i, best_j);
			ops[cnt].type = LW_PHI_RBAR;
			ops[cnt].i = best_i;
			ops[cnt].j = best_j;
			ops[cnt].k = -1;
		}
		else
		{
			apply_tpos(cur, n, best_i, best_j, best_k);
			ops[cnt].type = LW_PHI_RBBAR;
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
