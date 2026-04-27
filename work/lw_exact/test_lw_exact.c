#include "../foundations/foundations.h"
#include "lw_exact.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define LW_EXACT_REV   1
# define LW_EXACT_TPOS  2
# define LW_EXACT_BOTH  3
# define LW_EXACT_RBAR  4

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

static int	is_sorted_signed(int *p, int n)
{
	for (int i = 0; i < n; i++)
		if (p[i] != i + 1)
			return (0);
	return (1);
}

static void	apply_exact_ops(int *p, int n, LWExactOp *ops, int cnt)
{
	for (int k = 0; k < cnt; k++)
	{
		if (ops[k].type == LW_EXACT_RBAR)
			apply_srev(p, n, ops[k].i, ops[k].j);
		else if (ops[k].type == LW_EXACT_TPOS)
			apply_tpos(p, n, ops[k].i, ops[k].j, ops[k].k);
		else
			apply_rev(p, n, ops[k].i, ops[k].j);
	}
}

static void	test_already_sorted(void)
{
	int			p[] = {1, 2, 3};
	int			cnt;
	LWExactOp	*ops;

	ops = lw_exact(p, 3, 3.0, LW_EXACT_BOTH, &cnt);
	CHECK(cnt == 0, "lw_exact already sorted: 0 ops");
	free(ops);
}

static void	test_unsigned_rev(void)
{
	int			orig[] = {3, 2, 1};
	int			p[] = {3, 2, 1};
	int			cnt;
	LWExactOp	*ops;

	ops = lw_exact(orig, 3, 3.0, LW_EXACT_REV, &cnt);
	CHECK(cnt > 0, "lw_exact unsigned rev: ops applied");
	apply_exact_ops(p, 3, ops, cnt);
	CHECK(is_sorted_signed(p, 3), "lw_exact unsigned rev: sorted");
	/* cost = 2^3 * Inv(3,2,1) = 8 * 3 = 24 and cnt = Inv = 3 */
	CHECK(cnt == 3, "lw_exact unsigned rev: exactly Inv(pi) ops");
	free(ops);
}

static void	test_unsigned_tpos(void)
{
	int			orig[] = {2, 1, 4, 3};
	int			p[] = {2, 1, 4, 3};
	int			cnt;
	LWExactOp	*ops;

	ops = lw_exact(orig, 4, 3.0, LW_EXACT_TPOS, &cnt);
	apply_exact_ops(p, 4, ops, cnt);
	CHECK(is_sorted_signed(p, 4), "lw_exact unsigned tpos: sorted");
	CHECK(cnt == 2, "lw_exact unsigned tpos: exactly Inv(pi)=2 ops");
	free(ops);
}

static void	test_signed_rbar(void)
{
	/* {-2, +1, +3}: Inv=1, E-even=0, E+odd=1 → cost=2^3+1=9 */
	int			orig[] = {-2, 1, 3};
	int			p[] = {-2, 1, 3};
	int			cnt;
	LWExactOp	*ops;

	CHECK(is_lam_perm(orig, 3, 3), "lw_exact signed: is 3-perm");
	ops = lw_exact(orig, 3, 3.0, LW_EXACT_RBAR, &cnt);
	apply_exact_ops(p, 3, ops, cnt);
	CHECK(is_sorted_signed(p, 3), "lw_exact signed rbar: sorted");
	CHECK(cnt == 2, "lw_exact signed rbar: Inv=1 + 1 sign fix = 2 ops");
	free(ops);
}

static void	test_signed_all_neg(void)
{
	/* {-1, -2, -3}: Inv=0, needs 3 sign fixes */
	int			orig[] = {-1, -2, -3};
	int			p[] = {-1, -2, -3};
	int			cnt;
	LWExactOp	*ops;

	ops = lw_exact(orig, 3, 3.0, LW_EXACT_RBAR, &cnt);
	apply_exact_ops(p, 3, ops, cnt);
	CHECK(is_sorted_signed(p, 3), "lw_exact all neg: sorted");
	CHECK(cnt == 3, "lw_exact all neg: 3 sign fixes");
	free(ops);
}

int	main(void)
{
	test_already_sorted();
	test_unsigned_rev();
	test_unsigned_tpos();
	test_signed_rbar();
	test_signed_all_neg();
	printf("lw_exact: %d passed, %d failed\n", passed, failed);
	return (failed > 0 ? 1 : 0);
}
