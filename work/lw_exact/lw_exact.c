#include "../../includes/memory.h"
#include "../foundations/foundations.h"
#include "lw_exact.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*
 * [2020] Exact algorithms for alpha >= 3
 *
 * Key insight (Lemmas 23/24): for alpha >= 3, any large op can be
 * replaced by size-2 ops with no greater total cost.  The optimal
 * sorting sequence uses only super-short (size <= 2) operations.
 *
 * Theorem 9  (unsigned): optimal cost = 2^alpha * Inv(pi)
 *   → bubble sort with size-2 adjacent swaps
 *
 * Theorem 12 (signed reversals): optimal = 2^alpha*Inv + |E-_even| + |E+_odd|
 *   → sort absolute values with size-2 signed reversals,
 *     then fix wrong signs with size-1 signed reversals
 *
 * Theorem 13 (signed both, mode LW_EXACT_RBART): needs inversion graph
 *   codd computation — TODO
 */

# define LW_EXACT_REV    1
# define LW_EXACT_TPOS   2
# define LW_EXACT_BOTH   3
# define LW_EXACT_RBAR   4
# define LW_EXACT_RBART  5

static void	buf_push_ex(LWExactOp **ops, int *cnt, int *cap, int type, int i,
		int j, int k)
{
	if (*cnt >= *cap)
	{
		*cap *= 2;
		*ops = xrealloc(*ops, sizeof(LWExactOp) * (*cap / 2),
				sizeof(LWExactOp) * (*cap));
	}
	(*ops)[*cnt].type = type;
	(*ops)[*cnt].i = i;
	(*ops)[*cnt].j = j;
	(*ops)[*cnt].k = k;
	(*cnt)++;
}

/*
 * Theorem 9: bubble sort unsigned permutation using size-2 adjacent ops.
 * Each pass removes all adjacent inversions found left-to-right.
 */
static void	exact_unsigned(int *cur, int n, int mode, LWExactOp **ops,
		int *cnt, int *cap)
{
	int	swapped;

	do
	{
		swapped = 0;
		for (int i = 0; i < n - 1; i++)
		{
			if (abs(cur[i]) > abs(cur[i + 1]))
			{
				if (mode == LW_EXACT_TPOS)
				{
					apply_tpos(cur, n, i, i + 1, i + 2);
					buf_push_ex(ops, cnt, cap, LW_EXACT_TPOS, i, i + 1,
						i + 2);
				}
				else
				{
					apply_rev(cur, n, i, i + 1);
					buf_push_ex(ops, cnt, cap, LW_EXACT_REV, i, i + 1, -1);
				}
				swapped = 1;
			}
		}
	} while (swapped);
}

/*
 * Theorem 12: sort signed permutation.
 * Step 1: sort absolute values using size-2 signed reversals (ρ̄(i,i+1)).
 * Step 2: fix each wrong-signed element with size-1 signed reversal (ρ̄(i,i)).
 */
static void	exact_signed_rev(int *cur, int n, LWExactOp **ops, int *cnt,
		int *cap)
{
	int	swapped;

	/* Step 1: bubble sort on absolute values using signed reversals */
	do
	{
		swapped = 0;
		for (int i = 0; i < n - 1; i++)
		{
			if (abs(cur[i]) > abs(cur[i + 1]))
			{
				apply_srev(cur, n, i, i + 1);
				buf_push_ex(ops, cnt, cap, LW_EXACT_RBAR, i, i + 1, -1);
				swapped = 1;
			}
		}
	} while (swapped);
	/* Step 2: fix wrong signs */
	for (int i = 0; i < n; i++)
	{
		if (cur[i] < 0)
		{
			apply_srev(cur, n, i, i);
			buf_push_ex(ops, cnt, cap, LW_EXACT_RBAR, i, i, -1);
		}
	}
}

LWExactOp	*lw_exact(int *perm, int n, double alpha, int mode, int *op_count)
{
	int			cap;
	LWExactOp	*ops;
	int			cnt;
	int			*cur;

	(void)alpha;
	cap = n * n + 4;
	ops = xmalloc(sizeof(LWExactOp) * cap);
	cnt = 0;
	cur = xmalloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	if (mode == LW_EXACT_REV || mode == LW_EXACT_TPOS || mode == LW_EXACT_BOTH)
		exact_unsigned(cur, n, mode, &ops, &cnt, &cap);
	else if (mode == LW_EXACT_RBAR)
		exact_signed_rev(cur, n, &ops, &cnt, &cap);
	else
	{
		/* LW_EXACT_RBART (Theorem 13): needs inversion graph — TODO */
	}
	xfree(cur, sizeof(int) * n);
	*op_count = cnt;
	return (ops);
}
