#include "foundations.h"
#include <stdio.h>

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

static void	test_reversal(void)
{
	int	p[] = {1, 2, 3, 4, 5};
	int	s[] = {1, -2, -3, -4, 5};

	apply_rev(p, 5, 1, 3);
	CHECK(p[0] == 1 && p[1] == 4 && p[2] == 3 && p[3] == 2 && p[4] == 5,
		"apply_rev: [1,2,3,4,5] rev[1..3] → [1,4,3,2,5]");
	apply_srev(s, 5, 1, 3);
	CHECK(s[0] == 1 && s[1] == 4 && s[2] == 3 && s[3] == 2 && s[4] == 5,
		"apply_srev: [1,-2,-3,-4,5] srev[1..3] → [1,4,3,2,5]");
}

static void	test_transposition(void)
{
	int	p[] = {1, 2, 3, 4, 5};

	apply_tpos(p, 5, 0, 2, 5);
	CHECK(p[0] == 3 && p[1] == 4 && p[2] == 5 && p[3] == 1 && p[4] == 2,
		"apply_tpos: τ(0,2,5) → [3,4,5,1,2]");
}

static void	test_inversions(void)
{
	int	p[] = {3, 4, 1, 2};
	int	q[] = {1, 2, 3, 4};
	int	d;

	CHECK(cnt_inv(p, 4) == 4, "cnt_inv (3 4 1 2) = 4");
	CHECK(cnt_inv(q, 4) == 0, "cnt_inv identity = 0");
	d = delta_inv_rev(p, 4, 0, 1);
	CHECK(d == -1, "delta_inv_rev (3 4 1 2) ρ(0,1): -1");
	d = delta_inv_tpos(p, 4, 0, 2, 4);
	CHECK(d == 4, "delta_inv_tpos (3 4 1 2) τ(0,2,4): 4");
}

static void	test_entropy(void)
{
	int	p[] = {5, -4, 3, -1, 2};

	CHECK(ent_elem(5, 0) == 4, "ent_elem(5 at pos0) = 4");
	CHECK(ent_perm(p, 5) == 12, "ent_perm (+5 -4 +3 -1 +2) = 12");
	CHECK(cnt_Eminus(p, 5) == 1, "cnt_Eminus: -4 at pos1 even ent → 1");
	CHECK(cnt_Eplus(p, 5) == 1, "cnt_Eplus: only +2 at pos4 has odd ent3 → 1");
}

static void	test_is_lam_perm(void)
{
	int	p[] = {2, 1, 3};
	int	q[] = {3, 1, 2};

	CHECK(is_lam_perm(p, 3, 2) == 1, "is_lam_perm (2 1 3) λ=2 → yes");
	CHECK(is_lam_perm(q, 3, 2) == 0, "is_lam_perm (3 1 2) λ=2 → no");
}

static void	test_strips(void)
{
	int		p[] = {6, 4, 5, 3, 2, 1};
	int		cnt;
	Strip	*s;

	s = find_strips(p, 6, 0, &cnt);
	CHECK(cnt == 3, "find_strips (6 4 5 3 2 1): 3 strips");
	CHECK(s[0].start == 0 && s[0].end == 0 && s[0].is_increasing == 1,
		"strip0: (6) increasing");
	CHECK(s[1].start == 1 && s[1].end == 2 && s[1].is_increasing == 1,
		"strip1: (4 5) increasing");
	CHECK(s[2].start == 3 && s[2].end == 5 && s[2].is_increasing == 0,
		"strip2: (3 2 1) decreasing");
	xfree(s, sizeof(Strip) * 6);
}

static void	test_bp(void)
{
	int	p[] = {1, 5, 4, 3, 2};
	int	q[] = {1, 2, 3, 4, 5};

	CHECK(cnt_bp_rev(p, 5) == 2, "cnt_bp_rev (1 5 4 3 2) = 2");
	CHECK(cnt_bp_rev(q, 5) == 0, "cnt_bp_rev identity = 0");
	CHECK(cnt_bp_tpos(p, 5) == 5, "cnt_bp_tpos (1 5 4 3 2) = 5");
}

static void	test_codd(void)
{
	int	id[] = {1, 2, 3, 4};
	int	p2[] = {2, 1};
	int	p3[] = {2, 1, 3};
	int	p4[] = {3, 2, 1};
	int	ps[] = {-2, 1};

	/* identity: no inversions → n singleton components (all size 1, odd) */
	CHECK(codd_perm(id, 4) == 4, "codd identity(4): 4 singletons, all odd");
	/* one inversion: edge {2,1} → one even component {1,2}, rest singletons */
	CHECK(codd_perm(p2, 2) == 0, "codd (2,1): one even component, 0 odd");
	/* (2,1,3): edge {2,1} → {1,2} even, {3} odd → codd=1 */
	CHECK(codd_perm(p3, 3) == 1, "codd (2,1,3): 1 odd component");
	/* (3,2,1): edges {3,2},{3,1},{2,1} → all three in one component,
		size 3 odd */
	CHECK(codd_perm(p4, 3) == 1, "codd (3,2,1): one component size 3");
	/* signed: codd uses |.|, so (-2,+1) same as (2,1) */
	CHECK(codd_perm(ps, 2) == 0, "codd (-2,+1): same as (2,1), 0 odd");
}

int	main(void)
{
	test_reversal();
	test_transposition();
	test_inversions();
	test_entropy();
	test_is_lam_perm();
	test_strips();
	test_bp();
	test_codd();
	printf("foundations: %d passed, %d failed\n", passed, failed);
	return (failed > 0 ? 1 : 0);
}
