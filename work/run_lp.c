/*
 * run_lp — Miranda (2020/2021) λ-permutation algorithms demo
 *
 * Usage: ./run_lp <lambda> <e1> <e2> ... <en>
 *
 * Positive elements → unsigned permutation (Theorems 2, 5, 4,
	6; [2020] Algs 1,2,9)
 * Negative elements → signed permutation  (Theorems 3; [2020] Algs 3,12)
 *
 * Length-weighted algorithms use alpha=1.0 (adjustable at compile time).
 */

#include "foundations/foundations.h"
#include "lp_breakpoints/lp_breakpoints.h"
#include "lp_inversions/lp_inversions.h"
#include "lp_score/lp_score.h"
#include "lw_entropy/lw_entropy.h"
#include "lw_exact/lw_exact.h"
#include "lw_inversion/lw_inversion.h"
#include "lw_phi/lw_phi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LW_ALPHA 1.0
#define LW_EXACT_REV 1
#define LW_EXACT_TPOS 2
#define LW_EXACT_BOTH 3
#define LW_EXACT_RBAR 4
#define LW_EXACT_RBART 5

/* ── print helpers ──────────────────────────────────────────────── */

static void	print_perm(int *p, int n)
{
	printf("(");
	for (int i = 0; i < n; i++)
	{
		if (p[i] > 0)
			printf(" +%d", p[i]);
		else
			printf(" %d", p[i]);
	}
	printf(" )");
}

static int	is_sorted_signed(int *p, int n)
{
	for (int i = 0; i < n; i++)
		if (p[i] != i + 1)
			return (0);
	return (1);
}

static int	has_negative(int *p, int n)
{
	for (int i = 0; i < n; i++)
		if (p[i] < 0)
			return (1);
	return (0);
}

/* ── apply + print one op ───────────────────────────────────────── */

static void	apply_and_show_lp(int *cur, int n, LPOp *ops, int cnt)
{
	for (int k = 0; k < cnt; k++)
	{
		printf("  Step %2d:  ", k + 1);
		if (ops[k].type == LP_MODE_REV)
		{
			apply_rev(cur, n, ops[k].i, ops[k].j);
			printf("ρ(%d,%d)    ", ops[k].i, ops[k].j);
		}
		else
		{
			apply_tpos(cur, n, ops[k].i, ops[k].j, ops[k].k);
			printf("τ(%d,%d,%d) ", ops[k].i, ops[k].j, ops[k].k);
		}
		printf("→  ");
		print_perm(cur, n);
		printf("\n");
	}
}

static void	apply_and_show_score(int *cur, int n, LPScoreOp *ops, int cnt)
{
	for (int k = 0; k < cnt; k++)
	{
		printf("  Step %2d:  ", k + 1);
		if (ops[k].type == LP_SCORE_REV)
		{
			apply_srev(cur, n, ops[k].i, ops[k].j);
			printf("ρ̃(%d,%d)   ", ops[k].i, ops[k].j);
		}
		else
		{
			apply_tpos(cur, n, ops[k].i, ops[k].j, ops[k].k);
			printf("τ(%d,%d,%d) ", ops[k].i, ops[k].j, ops[k].k);
		}
		printf("→  ");
		print_perm(cur, n);
		printf("\n");
	}
}

static void	apply_and_show_bp(int *cur, int n, BPOp *ops, int cnt)
{
	for (int k = 0; k < cnt; k++)
	{
		printf("  Step %2d:  ", k + 1);
		if (ops[k].type == BP_TYPE_REV)
		{
			apply_rev(cur, n, ops[k].i, ops[k].j);
			printf("ρ(%d,%d)    ", ops[k].i, ops[k].j);
		}
		else
		{
			apply_tpos(cur, n, ops[k].i, ops[k].j, ops[k].k);
			printf("τ(%d,%d,%d) ", ops[k].i, ops[k].j, ops[k].k);
		}
		printf("→  ");
		print_perm(cur, n);
		printf("\n");
	}
}

static void	print_result(int *cur, int n, int cnt)
{
	if (is_sorted_signed(cur, n))
		printf("  → Sorted in %d operation%s\n", cnt, cnt == 1 ? "" : "s");
	else
	{
		printf("  → NOT sorted after %d ops! Remaining: ", cnt);
		print_perm(cur, n);
		printf("\n");
	}
}

static void	separator(const char *label)
{
	int	len;

	printf("\n── %s ", label);
	len = 50 - (int)strlen(label);
	while (len-- > 0)
		printf("─");
	printf("\n");
}

/* ── per-algorithm runners ──────────────────────────────────────── */

static void	run_lp_inv(int *perm, int n, int lambda, int mode,
		const char *label)
{
	int		cnt;
	LPOp	*ops;
	int		*cur;

	separator(label);
	cur = malloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	ops = lp_inv_greedy(perm, n, lambda, mode, &cnt);
	if (cnt == 0)
		printf("  (already sorted — 0 operations)\n");
	else
		apply_and_show_lp(cur, n, ops, cnt);
	print_result(cur, n, cnt);
	free(ops);
	free(cur);
}

static void	run_lp_score(int *perm, int n, int lambda, int mode,
		const char *label)
{
	int			cnt;
	LPScoreOp	*ops;
	int			*cur;

	separator(label);
	cur = malloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	ops = lp_score_greedy(perm, n, lambda, mode, &cnt);
	if (cnt == 0)
		printf("  (already sorted — 0 operations)\n");
	else
		apply_and_show_score(cur, n, ops, cnt);
	print_result(cur, n, cnt);
	free(ops);
	free(cur);
}

static void	run_lp_bp_tpos(int *perm, int n, int lambda, const char *label)
{
	int		cnt;
	BPOp	*ops;
	int		*cur;

	separator(label);
	cur = malloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	ops = lp_bp_tpos(perm, n, lambda, &cnt);
	if (cnt == 0)
		printf("  (already sorted — 0 operations)\n");
	else
		apply_and_show_bp(cur, n, ops, cnt);
	print_result(cur, n, cnt);
	free(ops);
	free(cur);
}

static void	run_lp_bp_both(int *perm, int n, int lambda, int is_signed,
		const char *label)
{
	int		cnt;
	BPOp	*ops;
	int		*cur;

	separator(label);
	cur = malloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	ops = lp_bp_both(perm, n, lambda, is_signed, &cnt);
	if (cnt == 0)
		printf("  (already sorted — 0 operations)\n");
	else
		apply_and_show_bp(cur, n, ops, cnt);
	print_result(cur, n, cnt);
	free(ops);
	free(cur);
}

static void	run_lp_bp_rev(int *perm, int n, int lambda, int is_signed,
		const char *label)
{
	int		cnt;
	BPOp	*ops;
	int		*cur;

	separator(label);
	cur = malloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	ops = lp_bp_rev(perm, n, lambda, is_signed, &cnt);
	if (cnt == 0)
		printf("  (already sorted — 0 operations)\n");
	else
		apply_and_show_bp(cur, n, ops, cnt);
	print_result(cur, n, cnt);
	free(ops);
	free(cur);
}

/* ── [2020] length-weighted runners ────────────────────────────── */

static void	run_lw_inv(int *perm, int n, int lambda, int mode,
		const char *label)
{
	int		cnt;
	LWOp	*ops;
	int		*cur;

	separator(label);
	cur = malloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	ops = lw_inv_greedy(perm, n, lambda, LW_ALPHA, mode, &cnt);
	if (cnt == 0)
		printf("  (already sorted — 0 operations)\n");
	else
	{
		for (int k = 0; k < cnt; k++)
		{
			printf("  Step %2d:  ", k + 1);
			if (ops[k].type == LW_MODE_REV)
			{
				apply_rev(cur, n, ops[k].i, ops[k].j);
				printf("ρ(%d,%d)    ", ops[k].i, ops[k].j);
			}
			else
			{
				apply_tpos(cur, n, ops[k].i, ops[k].j, ops[k].k);
				printf("τ(%d,%d,%d) ", ops[k].i, ops[k].j, ops[k].k);
			}
			printf("→  ");
			print_perm(cur, n);
			printf("\n");
		}
	}
	print_result(cur, n, cnt);
	free(ops);
	free(cur);
}

static void	run_lw_ent(int *perm, int n, int lambda, int mode,
		const char *label)
{
	int		cnt;
	LWEntOp	*ops;
	int		*cur;

	separator(label);
	cur = malloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	ops = lw_ent_greedy(perm, n, lambda, LW_ALPHA, mode, &cnt);
	if (cnt == 0)
		printf("  (already sorted — 0 operations)\n");
	else
	{
		for (int k = 0; k < cnt; k++)
		{
			printf("  Step %2d:  ", k + 1);
			if (ops[k].type == LW_ENT_MODE_REV)
			{
				apply_rev(cur, n, ops[k].i, ops[k].j);
				printf("ρ(%d,%d)    ", ops[k].i, ops[k].j);
			}
			else
			{
				apply_tpos(cur, n, ops[k].i, ops[k].j, ops[k].k);
				printf("τ(%d,%d,%d) ", ops[k].i, ops[k].j, ops[k].k);
			}
			printf("→  ");
			print_perm(cur, n);
			printf("\n");
		}
	}
	print_result(cur, n, cnt);
	free(ops);
	free(cur);
}

static void	run_lw_phi(int *perm, int n, int lambda, const char *label)
{
	int		cnt;
	LWPhiOp	*ops;
	int		*cur;

	separator(label);
	cur = malloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	ops = lw_phi_greedy(perm, n, lambda, LW_ALPHA, LW_PHI_RBAR, &cnt);
	if (cnt == 0)
		printf("  (already sorted — 0 operations)\n");
	else
	{
		for (int k = 0; k < cnt; k++)
		{
			printf("  Step %2d:  ", k + 1);
			if (ops[k].type == LW_PHI_RBAR)
			{
				apply_srev(cur, n, ops[k].i, ops[k].j);
				printf("ρ̃(%d,%d)   ", ops[k].i, ops[k].j);
			}
			else
			{
				apply_tpos(cur, n, ops[k].i, ops[k].j, ops[k].k);
				printf("τ(%d,%d,%d) ", ops[k].i, ops[k].j, ops[k].k);
			}
			printf("→  ");
			print_perm(cur, n);
			printf("\n");
		}
	}
	print_result(cur, n, cnt);
	free(ops);
	free(cur);
}

static void	run_lw_psi(int *perm, int n, const char *label)
{
	int		cnt;
	LWPhiOp	*ops;
	int		*cur;

	separator(label);
	cur = malloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	ops = lw_psi_greedy_short(perm, n, &cnt);
	if (cnt == 0)
		printf("  (already sorted — 0 operations)\n");
	else
	{
		for (int k = 0; k < cnt; k++)
		{
			printf("  Step %2d:  ", k + 1);
			if (ops[k].type == LW_PHI_RBAR)
			{
				apply_srev(cur, n, ops[k].i, ops[k].j);
				printf("ρ̃(%d,%d)   ", ops[k].i, ops[k].j);
			}
			else
			{
				apply_tpos(cur, n, ops[k].i, ops[k].j, ops[k].k);
				printf("τ(%d,%d,%d) ", ops[k].i, ops[k].j, ops[k].k);
			}
			printf("→  ");
			print_perm(cur, n);
			printf("\n");
		}
	}
	print_result(cur, n, cnt);
	free(ops);
	free(cur);
}

static void	run_lw_exact(int *perm, int n, int mode, const char *label)
{
	int			cnt;
	LWExactOp	*ops;
	int			*cur;

	separator(label);
	cur = malloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	ops = lw_exact(perm, n, 3.0, mode, &cnt);
	if (cnt == 0)
		printf("  (already sorted — 0 operations)\n");
	else
	{
		for (int k = 0; k < cnt; k++)
		{
			printf("  Step %2d:  ", k + 1);
			if (ops[k].type == LW_EXACT_RBAR)
			{
				apply_srev(cur, n, ops[k].i, ops[k].j);
				printf("ρ̃(%d,%d)   ", ops[k].i, ops[k].j);
			}
			else if (ops[k].type == LW_EXACT_TPOS)
			{
				apply_tpos(cur, n, ops[k].i, ops[k].j, ops[k].k);
				printf("τ(%d,%d,%d) ", ops[k].i, ops[k].j, ops[k].k);
			}
			else
			{
				apply_rev(cur, n, ops[k].i, ops[k].j);
				printf("ρ(%d,%d)    ", ops[k].i, ops[k].j);
			}
			printf("→  ");
			print_perm(cur, n);
			printf("\n");
		}
	}
	print_result(cur, n, cnt);
	free(ops);
	free(cur);
}

/* ── main ───────────────────────────────────────────────────────── */

int	main(int ac, char **av)
{
	int	lambda;
	int	n;
	int	*perm;
	int	is_signed;

	if (ac < 3)
	{
		printf("Usage: ./run_lp <lambda> <e1> <e2> ... <en>\n");
		printf("  e.g.: ./run_lp 3 3 1 2\n");
		printf("  e.g.: ./run_lp 3 -2 1 3        (signed)\n");
		return (1);
	}
	lambda = atoi(av[1]);
	n = ac - 2;
	perm = malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++)
		perm[i] = atoi(av[i + 2]);
	/* ── header ── */
	printf("Input:      ");
	print_perm(perm, n);
	printf("   n=%d  λ=%d\n", n, lambda);
	printf("λ-perm:     %s\n", is_lam_perm(perm, n,
			lambda) ? "YES" : "NO (result may be wrong)");
	printf("Inversions: %d\n", cnt_inv(perm, n));
	is_signed = has_negative(perm, n);
	if (is_signed)
		printf("Type:       signed\n");
	else
		printf("Type:       unsigned\n");
	if (is_sorted_signed(perm, n))
	{
		printf("\nAlready sorted — nothing to do.\n");
		free(perm);
		return (0);
	}
	/* ── run algorithms ── */
	if (!is_signed)
	{
		printf("\n── [2021] Miranda λ-permutation algorithms ──────────────────────\n");
		run_lp_inv(perm, n, lambda, LP_MODE_BOTH, "Thm 2  lp_inv  (rev+tpos,
			minimise inversions)");
		run_lp_bp_tpos(perm, n, lambda,
			"Thm 5  lp_bp_tpos  (transpositions only)");
		run_lp_bp_both(perm, n, lambda, 0, "Thm 4  lp_bp_both  (tpos + rev)");
		run_lp_bp_rev(perm, n, lambda, 0,
			"Thm 6  lp_bp_rev   (reversals only)");
		printf("\n── [2020] Miranda length-weighted (α=%.1f) ──────────────────────\n",
			LW_ALPHA);
		run_lw_inv(perm, n, lambda, LW_MODE_BOTH, "Alg 1  lw_inv  (rev+tpos,
			weighted ΔInv/|β|^α)");
		run_lw_ent(perm, n, lambda, LW_ENT_MODE_BOTH, "Alg 2  lw_ent  (rev+tpos,
			weighted Δent/|β|^α)");
		printf("\n── [2020] Exact (α=3,
			optimal cost = 2³·Inv) ───────────────────\n");
		run_lw_exact(perm, n, LW_EXACT_BOTH,
			"Thm 9  lw_exact  (size-2 adjacent swaps)");
	}
	else
	{
		printf("\n── [2021] Miranda λ-permutation algorithms (signed) ─────────────\n");
		run_lp_score(perm, n, lambda, LP_SCORE_REV,
			"Thm 3  lp_score  (signed rev, maximise score)");
		printf("\n── [2020] Miranda length-weighted signed (α=%.1f) ───────────────\n",
			LW_ALPHA);
		run_lw_phi(perm, n, lambda, "Alg 3  lw_phi  (signed rev,
			weighted Δ(2Inv+E)/|β|^α)");
		run_lw_psi(perm, n, "lw_psi (λ=3 signed rev+tpos,
			ψ-score Δ(2Inv+codd)/|β|^α)");
		printf("\n── [2020] Exact signed (α=3) ────────────────────────────────────\n");
		run_lw_exact(perm, n, LW_EXACT_RBAR,
			"Thm12  lw_exact RBAR   (size-2 signed rev + sign fix)");
		run_lw_exact(perm, n, LW_EXACT_RBART,
			"Thm13  lw_exact RBART  (size-2 signed rev+tpos + sign fix)");
	}
	printf("\n");
	free(perm);
	return (0);
}
