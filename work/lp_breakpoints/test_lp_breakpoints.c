#include "../foundations/foundations.h"
#include "lp_breakpoints.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int			passed = 0;
static int			failed = 0;

static inline void	chk(int cond, const char *msg)
{
	if (cond)
		passed++;
	else
	{
		failed++;
		printf("FAIL: %s\n", msg);
	}
}

#define CHECK(c, m) chk((c), (m))

static int	is_sorted(int *perm, int n)
{
	for (int i = 0; i < n; i++)
		if (abs(perm[i]) != i + 1)
			return (0);
	return (1);
}

static void	apply_ops(int *perm, int n, BPOp *ops, int cnt)
{
	for (int k = 0; k < cnt; k++)
	{
		if (ops[k].type == BP_TYPE_REV)
			apply_rev(perm, n, ops[k].i, ops[k].j);
		else
			apply_tpos(perm, n, ops[k].i, ops[k].j, ops[k].k);
	}
}

static void	test_tpos_already(void)
{
	int		p[] = {1, 2, 3};
	int		cnt;
	BPOp	*ops;

	ops = lp_bp_tpos(p, 3, 3, &cnt);
	CHECK(cnt == 0, "tpos_already: no ops needed");
	CHECK(is_sorted(p, 3), "tpos_already: still sorted");
	free(ops);
}

static void	test_tpos_simple(void)
{
	int		orig[] = {3, 1, 2};
	int		p[] = {3, 1, 2};
	int		cnt;
	BPOp	*ops;

	CHECK(is_lam_perm(orig, 3, 3), "tpos_simple: input is 3-perm");
	ops = lp_bp_tpos(orig, 3, 3, &cnt);
	CHECK(cnt > 0, "tpos_simple: ops applied");
	apply_ops(p, 3, ops, cnt);
	CHECK(is_sorted(p, 3), "tpos_simple: sorted to identity");
	free(ops);
}

static void	test_both_inc_strip(void)
{
	int		orig[] = {3, 1, 2};
	int		p[] = {3, 1, 2};
	int		cnt;
	BPOp	*ops;

	CHECK(is_lam_perm(orig, 3, 3), "both_inc: input is 3-perm");
	ops = lp_bp_both(orig, 3, 3, 0, &cnt);
	CHECK(cnt > 0, "both_inc: ops applied");
	apply_ops(p, 3, ops, cnt);
	CHECK(is_sorted(p, 3), "both_inc: sorted to identity");
	free(ops);
}

static void	test_both_dec_strip(void)
{
	int		orig[] = {3, 2, 1};
	int		p[] = {3, 2, 1};
	int		cnt;
	BPOp	*ops;

	/* (3 2 1): all one decreasing strip */
	CHECK(is_lam_perm(orig, 3, 3), "both_dec: input is 3-perm");
	ops = lp_bp_both(orig, 3, 3, 0, &cnt);
	apply_ops(p, 3, ops, cnt);
	CHECK(is_sorted(p, 3), "both_dec: sorted to identity");
	free(ops);
}

static void	test_rev_inc(void)
{
	int		orig[] = {2, 3, 1};
	int		p[] = {2, 3, 1};
	int		cnt;
	BPOp	*ops;

	CHECK(is_lam_perm(orig, 3, 3), "rev_inc: input is 3-perm");
	ops = lp_bp_rev(orig, 3, 3, 0, &cnt);
	CHECK(cnt > 0, "rev_inc: ops applied");
	apply_ops(p, 3, ops, cnt);
	CHECK(is_sorted(p, 3), "rev_inc: sorted to identity");
	free(ops);
}

static void	test_larger(void)
{
	int		orig[] = {2, 1, 4, 3};
	int		p[] = {2, 1, 4, 3};
	int		cnt;
	BPOp	*ops;

	/* 4-perm: (2 1 4 3) - two adjacent swaps, λ=3 */
	CHECK(is_lam_perm(orig, 4, 3), "larger: input is 3-perm");
	ops = lp_bp_tpos(orig, 4, 3, &cnt);
	apply_ops(p, 4, ops, cnt);
	CHECK(is_sorted(p, 4), "larger: sorted to identity");
	free(ops);
}

int	main(void)
{
	test_tpos_already();
	test_tpos_simple();
	test_both_inc_strip();
	test_both_dec_strip();
	test_rev_inc();
	test_larger();
	printf("lp_breakpoints: %d passed, %d failed\n", passed, failed);
	return (failed > 0 ? 1 : 0);
}
