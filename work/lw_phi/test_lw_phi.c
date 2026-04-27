#include "../foundations/foundations.h"
#include "lw_phi.h"
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

static int	is_sorted_signed(int *p, int n)
{
	for (int i = 0; i < n; i++)
		if (p[i] != i + 1)
			return (0);
	return (1);
}

static void	apply_phi_ops(int *p, int n, LWPhiOp *ops, int cnt)
{
	for (int k = 0; k < cnt; k++)
	{
		if (ops[k].type == LW_PHI_RBAR)
			apply_srev(p, n, ops[k].i, ops[k].j);
		else
			apply_tpos(p, n, ops[k].i, ops[k].j, ops[k].k);
	}
}

static void	test_already_sorted(void)
{
	int		p[] = {1, 2, 3};
	int		cnt;
	LWPhiOp	*ops;

	ops = lw_phi_greedy(p, 3, 3, 1.0, LW_PHI_RBAR, &cnt);
	CHECK(cnt == 0, "lw_phi already sorted: 0 ops");
	free(ops);
}

static void	test_single_neg_alpha1(void)
{
	int		orig[] = {-1, 2, 3};
	int		p[] = {-1, 2, 3};
	int		cnt;
	LWPhiOp	*ops;

	CHECK(is_lam_perm(orig, 3, 2), "lw_phi single neg: is 2-perm");
	ops = lw_phi_greedy(orig, 3, 2, 1.0, LW_PHI_RBAR, &cnt);
	CHECK(cnt > 0, "lw_phi single neg: ops applied");
	apply_phi_ops(p, 3, ops, cnt);
	CHECK(is_sorted_signed(p, 3), "lw_phi single neg: sorted");
	free(ops);
}

static void	test_signed_lam3_alpha1(void)
{
	int		orig[] = {-2, 1, 3};
	int		p[] = {-2, 1, 3};
	int		cnt;
	LWPhiOp	*ops;

	CHECK(is_lam_perm(orig, 3, 3), "lw_phi signed lam3: is 3-perm");
	ops = lw_phi_greedy(orig, 3, 3, 1.0, LW_PHI_RBAR, &cnt);
	CHECK(cnt > 0, "lw_phi signed lam3 alpha=1: ops applied");
	apply_phi_ops(p, 3, ops, cnt);
	CHECK(is_sorted_signed(p, 3), "lw_phi signed lam3 alpha=1: sorted");
	free(ops);
}

static void	test_signed_alpha2(void)
{
	int		orig[] = {-2, 1, 3};
	int		p[] = {-2, 1, 3};
	int		cnt;
	LWPhiOp	*ops;

	ops = lw_phi_greedy(orig, 3, 3, 2.0, LW_PHI_RBAR, &cnt);
	CHECK(cnt > 0, "lw_phi signed alpha=2: ops applied");
	apply_phi_ops(p, 3, ops, cnt);
	CHECK(is_sorted_signed(p, 3), "lw_phi signed alpha=2: sorted");
	free(ops);
}

static void	test_psi_sorted(void)
{
	int		p[] = {1, 2, 3};
	int		cnt;
	LWPhiOp	*ops;

	ops = lw_psi_greedy_short(p, 3, &cnt);
	CHECK(cnt == 0, "lw_psi already sorted: 0 ops");
	free(ops);
}

static void	test_psi_sign_only(void)
{
	int		orig[] = {1, -2, 3};
	int		p[] = {1, -2, 3};
	int		cnt;
	LWPhiOp	*ops;

	ops = lw_psi_greedy_short(orig, 3, &cnt);
	CHECK(cnt > 0, "lw_psi sign-only: ops applied");
	apply_phi_ops(p, 3, ops, cnt);
	CHECK(is_sorted_signed(p, 3), "lw_psi sign-only: sorted");
	free(ops);
}

static void	test_psi_inv_and_sign(void)
{
	int		orig[] = {-2, 1, 3};
	int		p[] = {-2, 1, 3};
	int		cnt;
	LWPhiOp	*ops;

	CHECK(is_lam_perm(orig, 3, 3), "lw_psi inv+sign: is 3-perm");
	ops = lw_psi_greedy_short(orig, 3, &cnt);
	CHECK(cnt > 0, "lw_psi inv+sign: ops applied");
	apply_phi_ops(p, 3, ops, cnt);
	CHECK(is_sorted_signed(p, 3), "lw_psi inv+sign: sorted");
	free(ops);
}

static void	test_psi_unsigned_both(void)
{
	int		orig[] = {3, 1, 2};
	int		p[] = {3, 1, 2};
	int		cnt;
	LWPhiOp	*ops;

	CHECK(is_lam_perm(orig, 3, 3), "lw_psi unsigned both: is 3-perm");
	ops = lw_psi_greedy_short(orig, 3, &cnt);
	CHECK(cnt > 0, "lw_psi unsigned both: ops applied");
	apply_phi_ops(p, 3, ops, cnt);
	CHECK(is_sorted_signed(p, 3), "lw_psi unsigned both: sorted");
	free(ops);
}

int	main(void)
{
	test_already_sorted();
	test_single_neg_alpha1();
	test_signed_lam3_alpha1();
	test_signed_alpha2();
	test_psi_sorted();
	test_psi_sign_only();
	test_psi_inv_and_sign();
	test_psi_unsigned_both();
	printf("lw_phi: %d passed, %d failed\n", passed, failed);
	return (failed > 0 ? 1 : 0);
}
