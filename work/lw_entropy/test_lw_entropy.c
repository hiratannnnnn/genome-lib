#include "../foundations/foundations.h"
#include "lw_entropy.h"
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

static int	is_sorted(int *p, int n)
{
	for (int i = 0; i < n; i++)
		if (abs(p[i]) != i + 1)
			return (0);
	return (1);
}

static void	apply_ent_ops(int *p, int n, LWEntOp *ops, int cnt)
{
	for (int k = 0; k < cnt; k++)
	{
		if (ops[k].type == LW_ENT_MODE_REV)
			apply_rev(p, n, ops[k].i, ops[k].j);
		else
			apply_tpos(p, n, ops[k].i, ops[k].j, ops[k].k);
	}
}

static void	test_already_sorted(void)
{
	int		p[] = {1, 2, 3};
	int		cnt;
	LWEntOp	*ops;

	ops = lw_ent_greedy(p, 3, 3, 1.0, LW_ENT_MODE_BOTH, &cnt);
	CHECK(cnt == 0, "lw_ent already sorted: 0 ops");
	free(ops);
}

static void	test_adj_swap(void)
{
	int		orig[] = {2, 1, 3};
	int		p[] = {2, 1, 3};
	int		cnt;
	LWEntOp	*ops;

	ops = lw_ent_greedy(orig, 3, 3, 1.0, LW_ENT_MODE_REV, &cnt);
	CHECK(cnt > 0, "lw_ent adj swap: ops applied");
	apply_ent_ops(p, 3, ops, cnt);
	CHECK(is_sorted(p, 3), "lw_ent adj swap: sorted");
	free(ops);
}

static void	test_tpos_alpha1(void)
{
	int		orig[] = {3, 1, 2};
	int		p[] = {3, 1, 2};
	int		cnt;
	LWEntOp	*ops;

	CHECK(is_lam_perm(orig, 3, 3), "lw_ent tpos: is 3-perm");
	ops = lw_ent_greedy(orig, 3, 3, 1.0, LW_ENT_MODE_TPOS, &cnt);
	CHECK(cnt > 0, "lw_ent tpos alpha=1: ops applied");
	apply_ent_ops(p, 3, ops, cnt);
	CHECK(is_sorted(p, 3), "lw_ent tpos alpha=1: sorted");
	free(ops);
}

static void	test_lem9_fallback(void)
{
	int		orig[] = {3, 1, 2};
	int		p[] = {3, 1, 2};
	int		cnt;
	LWEntOp	*ops;

	/* alpha=10 makes all large ops unattractive; fallback to Lemma 9 */
	ops = lw_ent_greedy(orig, 3, 3, 10.0, LW_ENT_MODE_REV, &cnt);
	CHECK(cnt > 0, "lw_ent lem9 fallback: ops applied");
	apply_ent_ops(p, 3, ops, cnt);
	CHECK(is_sorted(p, 3), "lw_ent lem9 fallback: sorted");
	free(ops);
}

static void	test_larger(void)
{
	int		orig[] = {2, 3, 1, 5, 4};
	int		p[] = {2, 3, 1, 5, 4};
	int		cnt;
	LWEntOp	*ops;

	CHECK(is_lam_perm(orig, 5, 3), "lw_ent larger: is 3-perm");
	ops = lw_ent_greedy(orig, 5, 3, 1.0, LW_ENT_MODE_BOTH, &cnt);
	apply_ent_ops(p, 5, ops, cnt);
	CHECK(is_sorted(p, 5), "lw_ent larger: sorted");
	free(ops);
}

int	main(void)
{
	test_already_sorted();
	test_adj_swap();
	test_tpos_alpha1();
	test_lem9_fallback();
	test_larger();
	printf("lw_entropy: %d passed, %d failed\n", passed, failed);
	return (failed > 0 ? 1 : 0);
}
