#include "../foundations/foundations.h"
#include "lp_inversions.h"
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

static void	test_already_sorted(void)
{
	int		p[] = {1, 2, 3, 4};
	int		cnt;
	LPOp	*ops;

	ops = lp_inv_greedy(p, 4, 3, LP_MODE_BOTH, &cnt);
	CHECK(cnt == 0, "already sorted: 0 ops");
	if (ops)
		free(ops);
}

static int	verify_sorted(int *perm, int n)
{
	for (int i = 0; i < n; i++)
		if (perm[i] != i + 1)
			return (0);
	return (1);
}

static void	apply_ops(int *perm, int n, LPOp *ops, int cnt)
{
	for (int k = 0; k < cnt; k++)
	{
		if (ops[k].type == LP_MODE_REV)
			apply_rev(perm, n, ops[k].i, ops[k].j);
		else
			apply_tpos(perm, n, ops[k].i, ops[k].j, ops[k].k);
	}
}

static void	test_simple_rev(void)
{
	int		p[] = {2, 1, 3};
	int		orig[] = {2, 1, 3};
	int		cnt;
	LPOp	*ops;

	ops = lp_inv_greedy(orig, 3, 3, LP_MODE_REV, &cnt);
	CHECK(cnt > 0, "simple rev: needs ops");
	apply_ops(p, 3, ops, cnt);
	CHECK(verify_sorted(p, 3), "simple rev: result is identity");
	CHECK(is_lam_perm(p, 3, 3), "simple rev: result is λ-perm");
	free(ops);
}

static void	test_simple_tpos(void)
{
	int		p[] = {3, 1, 2};
	int		orig[] = {3, 1, 2};
	int		cnt;
	LPOp	*ops;

	ops = lp_inv_greedy(orig, 3, 3, LP_MODE_TPOS, &cnt);
	CHECK(cnt > 0, "simple tpos: needs ops");
	apply_ops(p, 3, ops, cnt);
	CHECK(verify_sorted(p, 3), "simple tpos: result is identity");
	free(ops);
}

static void	test_lam3(void)
{
	int		p[] = {2, 3, 1, 5, 4};
	int		orig[] = {2, 3, 1, 5, 4};
	int		cnt;
	LPOp	*ops;

	CHECK(is_lam_perm(orig, 5, 3), "test_lam3: input is 3-perm");
	ops = lp_inv_greedy(orig, 5, 3, LP_MODE_BOTH, &cnt);
	apply_ops(p, 5, ops, cnt);
	CHECK(verify_sorted(p, 5), "test_lam3: sorted to identity");
	free(ops);
}

int	main(void)
{
	test_already_sorted();
	test_simple_rev();
	test_simple_tpos();
	test_lam3();
	printf("lp_inversions: %d passed, %d failed\n", passed, failed);
	return (failed > 0 ? 1 : 0);
}
