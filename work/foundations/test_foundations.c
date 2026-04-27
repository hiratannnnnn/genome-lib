#include "foundations.h"
#include <assert.h>
#include <stdio.h>

static int	passed = 0;
static int	failed = 0;

#define CHECK(cond, msg)

do
{
	if (cond)
	{
		passed++;
	}
	else
	{
		failed++;
		printf("FAIL: %s\n", msg);
	}
} while (0)

	static void
	test_reversal(void)
{
	int	p[] = {1, 2, 3, 4, 5};

	apply_rev(p, 5, 1, 3); /* reverse [2,3,4] → [4,3,2] */
	CHECK(p[0] == 1 && p[1] == 4 && p[2] == 3 && p[3] == 2 && p[4] == 5,
		"apply_rev basic");
}

static void	test_transposition(void)
{
	int	p[] = {1, 2, 3, 4, 5};

	apply_tpos(p, 5, 0, 2, 5); /* swap [1,2] and [3,4,5] */
	CHECK(p[0] == 3 && p[1] == 4 && p[2] == 5 && p[3] == 1 && p[4] == 2,
		"apply_tpos basic");
}

static void	test_inversions(void)
{
	int	p[] = {3, 4, 1, 2};
	int	q[] = {1, 2, 3, 4};

	CHECK(cnt_inv(p, 4) == 4, "cnt_inv (3 4 1 2) = 4");
	CHECK(cnt_inv(q, 4) == 0, "cnt_inv identity = 0");
}

static void	test_entropy(void)
{
	int	p[] = {5, -4, 3, -1, 2};
	int	e;

	/* ent(+5 at pos 0) = |5-1| = 4 */
	CHECK(ent_elem(5, 0) == 4, "ent_elem");
	e = ent_perm(p, 5);
	/* |5-1|+|-4|-2|+|3-3|+|-1|-4|+|2-5| = 4+2+0+3+3 = 12 */
	CHECK(e == 12, "ent_perm (+5 -4 +3 -1 +2) = 12");
}

static void	test_is_lam_perm(void)
{
	int	p[] = {2, 1, 3};
	int	q[] = {3, 1, 2};

	/* (2 1 3) with lambda=2: ||2|-1|=1<2, ||1|-2|=1<2, ||3|-3|=0<2 → yes */
	CHECK(is_lam_perm(p, 3, 2) == 1, "is_lam_perm (2 1 3), λ=2 → yes");
	/* (3 1 2) with lambda=2: ||3|-1|=2 ≥ 2 → no */
	CHECK(is_lam_perm(q, 3, 2) == 0, "is_lam_perm (3 1 2), λ=2 → no");
}

int	main(void)
{
	test_reversal();
	test_transposition();
	test_inversions();
	test_entropy();
	test_is_lam_perm();
	printf("foundations: %d passed, %d failed\n", passed, failed);
	return (failed > 0 ? 1 : 0);
}
