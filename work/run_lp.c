/*
 * run_lp — Miranda (2021) λ-permutation algorithms demo
 *
 * Usage: ./run_lp <lambda> <e1> <e2> ... <en>
 *
 * Positive elements → unsigned permutation (Theorems 2, 5, 4, 6)
 * Negative elements → signed permutation  (Theorems 3, 4, 6)
 */

#include "foundations/foundations.h"
#include "lp_inversions/lp_inversions.h"
#include "lp_breakpoints/lp_breakpoints.h"
#include "lp_score/lp_score.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	printf("\n── %s ", label);
	int	len = 50 - (int)strlen(label);
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

/* ── main ───────────────────────────────────────────────────────── */

int	main(int ac, char **av)
{
	if (ac < 3)
	{
		printf("Usage: ./run_lp <lambda> <e1> <e2> ... <en>\n");
		printf("  e.g.: ./run_lp 3 3 1 2\n");
		printf("  e.g.: ./run_lp 3 -2 1 3        (signed)\n");
		return (1);
	}
	int	lambda = atoi(av[1]);
	int	n = ac - 2;
	int	*perm = malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++)
		perm[i] = atoi(av[i + 2]);

	/* ── header ── */
	printf("Input:      ");
	print_perm(perm, n);
	printf("   n=%d  λ=%d\n", n, lambda);
	printf("λ-perm:     %s\n", is_lam_perm(perm, n, lambda) ? "YES" : "NO (result may be wrong)");
	printf("Inversions: %d\n", cnt_inv(perm, n));

	int	is_signed = has_negative(perm, n);
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
		run_lp_inv(perm, n, lambda, LP_MODE_BOTH,
			"Theorem 2  lp_inv  (rev + tpos, minimise inversions)");
		run_lp_bp_tpos(perm, n, lambda,
			"Theorem 5  lp_bp_tpos  (transpositions only)");
		run_lp_bp_both(perm, n, lambda, 0,
			"Theorem 4  lp_bp_both  (tpos + rev)");
		run_lp_bp_rev(perm, n, lambda, 0,
			"Theorem 6  lp_bp_rev   (reversals only)");
	}
	else
	{
		/* Theorem 3: score-based greedy for signed permutations */
		run_lp_score(perm, n, lambda, LP_SCORE_REV,
			"Theorem 3  lp_score  (signed rev, maximise score)");
		printf("\n  (Theorems 4/6 for signed inputs: TODO — needs apply_srev)\n");
	}

	printf("\n");
	free(perm);
	return (0);
}
