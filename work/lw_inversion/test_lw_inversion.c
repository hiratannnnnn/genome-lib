#include "../foundations/foundations.h"
#include "lw_inversion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int	passed = 0;
static int	failed = 0;

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

static int	is_sorted(int *p, int n)
{
	for (int i = 0; i < n; i++)
		if (abs(p[i]) != i + 1)
			return (0);
	return (1);
}

static void	apply_lw_ops(int *p, int n, LWOp *ops, int cnt)
{
	for (int k = 0; k < cnt; k++)
	{
		if (ops[k].type == LW_MODE_REV)
			apply_rev(p, n, ops[k].i, ops[k].j);
		else
			apply_tpos(p, n, ops[k].i, ops[k].j, ops[k].k);
	}
}

static void	test_already_sorted(void)
{
	int		p[] = {1, 2, 3};
	int		cnt;
	LWOp	*ops;

	ops = lw_inv_greedy(p, 3, 3, 1.0, LW_MODE_BOTH, &cnt);
	CHECK(cnt == 0, "lw_inv already sorted: 0 ops");
	free(ops);
}

static void	test_rev_alpha1(void)
{
	int		orig[] = {2, 1, 3};
	int		p[] = {2, 1, 3};
	int		cnt;
	LWOp	*ops;

	ops = lw_inv_greedy(orig, 3, 3, 1.0, LW_MODE_REV, &cnt);
	CHECK(cnt > 0, "lw_inv rev alpha=1: ops applied");
	apply_lw_ops(p, 3, ops, cnt);
	CHECK(is_sorted(p, 3), "lw_inv rev alpha=1: sorted");
	free(ops);
}

static void	test_tpos_alpha1(void)
{
	int		orig[] = {3, 1, 2};
	int		p[] = {3, 1, 2};
	int		cnt;
	LWOp	*ops;

	ops = lw_inv_greedy(orig, 3, 3, 1.0, LW_MODE_TPOS, &cnt);
	CHECK(cnt > 0, "lw_inv tpos alpha=1: ops applied");
	apply_lw_ops(p, 3, ops, cnt);
	CHECK(is_sorted(p, 3), "lw_inv tpos alpha=1: sorted");
	free(ops);
}

static void	test_both_alpha2(void)
{
	int		orig[] = {2, 3, 1, 5, 4};
	int		p[] = {2, 3, 1, 5, 4};
	int		cnt;
	LWOp	*ops;

	CHECK(is_lam_perm(orig, 5, 3), "lw_inv both alpha=2: is 3-perm");
	ops = lw_inv_greedy(orig, 5, 3, 2.0, LW_MODE_BOTH, &cnt);
	CHECK(cnt > 0, "lw_inv both alpha=2: ops applied");
	apply_lw_ops(p, 5, ops, cnt);
	CHECK(is_sorted(p, 5), "lw_inv both alpha=2: sorted");
	free(ops);
}

/* alpha=3: prefers shorter ops — with {2,1,3} lambda=3,
 * the size-2 reversal gets score 1/2^3=0.125 but it's the only beneficial op */
static void	test_alpha3(void)
{
	int		orig[] = {2, 1, 4, 3};
	int		p[] = {2, 1, 4, 3};
	int		cnt;
	LWOp	*ops;

	CHECK(is_lam_perm(orig, 4, 3), "lw_inv alpha=3: is 3-perm");
	ops = lw_inv_greedy(orig, 4, 3, 3.0, LW_MODE_BOTH, &cnt);
	apply_lw_ops(p, 4, ops, cnt);
	CHECK(is_sorted(p, 4), "lw_inv alpha=3: sorted");
	free(ops);
}

int	main(void)
{
	test_already_sorted();
	test_rev_alpha1();
	test_tpos_alpha1();
	test_both_alpha2();
	test_alpha3();
	printf("lw_inversion: %d passed, %d failed\n", passed, failed);
	return (failed > 0 ? 1 : 0);
}
