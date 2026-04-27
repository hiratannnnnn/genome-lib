#include "../foundations/foundations.h"
#include "lp_score.h"
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

static int	is_sorted_signed(int *perm, int n)
{
	for (int i = 0; i < n; i++)
		if (perm[i] != i + 1)
			return (0);
	return (1);
}

static void	apply_ops(int *perm, int n, LPScoreOp *ops, int cnt)
{
	for (int k = 0; k < cnt; k++)
	{
		if (ops[k].type == LP_SCORE_REV)
			apply_srev(perm, n, ops[k].i, ops[k].j);
		else
			apply_tpos(perm, n, ops[k].i, ops[k].j, ops[k].k);
	}
}

static void	test_already_sorted(void)
{
	int			p[] = {1, 2, 3};
	int			cnt;
	LPScoreOp	*ops;

	ops = lp_score_greedy(p, 3, 3, LP_SCORE_REV, &cnt);
	CHECK(cnt == 0, "already sorted: 0 ops");
	if (ops)
		free(ops);
}

static void	test_single_neg(void)
{
	int			p[] = {-1, 2, 3};
	int			orig[] = {-1, 2, 3};
	int			cnt;
	LPScoreOp	*ops;

	CHECK(is_lam_perm(orig, 3, 2), "single_neg: input is 2-perm");
	ops = lp_score_greedy(orig, 3, 2, LP_SCORE_REV, &cnt);
	apply_ops(p, 3, ops, cnt);
	CHECK(is_sorted_signed(p, 3), "single_neg: sorts to identity");
	free(ops);
}

static void	test_signed_lam3(void)
{
	int			p[] = {-2, 1, 3};
	int			orig[] = {-2, 1, 3};
	int			cnt;
	LPScoreOp	*ops;

	/* (-2 +1 +3): 2-perm, signed reversals only */
	CHECK(is_lam_perm(orig, 3, 3), "signed_lam3: is 3-perm");
	ops = lp_score_greedy(orig, 3, 3, LP_SCORE_REV, &cnt);
	apply_ops(p, 3, ops, cnt);
	CHECK(is_sorted_signed(p, 3), "signed_lam3: sorts to identity");
	free(ops);
}

int	main(void)
{
	test_already_sorted();
	test_single_neg();
	test_signed_lam3();
	printf("lp_score: %d passed, %d failed\n", passed, failed);
	return (failed > 0 ? 1 : 0);
}
