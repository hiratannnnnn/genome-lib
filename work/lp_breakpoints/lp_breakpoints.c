#include "../../includes/memory.h"
#include "lp_breakpoints.h"
#include <stdlib.h>
#include <string.h>

/*
 * [2021] Theorems 4, 5, 6 — breakpoints-based algorithms.
 *
 * All three share the outer loop:
 *   while b(π) > 0:
 *     find strip S containing the smallest out-of-place element
 *     move S to its correct position (Lemmas 7, 8, 9)
 *
 * Lemma 7:  move increasing strip with ≤4 λ-transpositions
 * Lemma 8:  move increasing strip with ≤(5+λ−1) λ-reversals
 * Lemma 9:  handle decreasing strip with ≤1 λ-tpos + ≤1 λ-rev
 * Lemma 10: reverse a decreasing strip → gives all-increasing perm
 *           (used as pre-processing step in Theorem 4)
 */

/* ── helpers ──────────────────────────────────────────────────────── */

static void	buf_push(BPOp **ops, int *cnt, int *cap, int type, int i, int j,
		int k)
{
	if (*cnt >= *cap)
	{
		*cap *= 2;
		*ops = xrealloc(*ops, sizeof(BPOp) * (*cap / 2), sizeof(BPOp) * (*cap));
	}
	(*ops)[*cnt].type = type;
	(*ops)[*cnt].i = i;
	(*ops)[*cnt].j = j;
	(*ops)[*cnt].k = k;
	(*cnt)++;
}

/* Lemma 10: reverse decreasing strip S in-place; result stays λ-perm. */
int	rev_dec_strip(int *perm, int n, int lambda, Strip *s)
{
	(void)lambda;
	(void)n;
	apply_rev(perm, n, s->start, s->end);
	return (1);
}

/*
 * Lemma 9: move decreasing strip S = (π_j … π_k) containing smallest
 * out-of-place element i = |π_k| to its correct position.
 * j = correct position of smallest element = i-1 (0-indexed).
 * When s->start == j: one reversal ρ(j, s->end) suffices.
 * Otherwise: τ(j, s->start, s->end+1) then ρ(j, j+(s->end-s->start)).
 * Returns number of ops applied.
 */
int	move_strip_dec(int *perm, int n, int lambda, Strip *s, BPOp *out)
{
	int	i;
	int	j;
	int	cnt;
	int	len;

	i = abs(perm[s->end]);
	j = i - 1;
	cnt = 0;
	if (s->start == j)
	{
		if (s->end - s->start + 1 <= lambda)
		{
			apply_rev(perm, n, j, s->end);
			out[cnt].type = BP_TYPE_REV;
			out[cnt].i = j;
			out[cnt].j = s->end;
			out[cnt].k = -1;
			cnt++;
		}
	}
	else
	{
		len = s->end - s->start;
		if (s->end + 1 - j <= lambda)
		{
			apply_tpos(perm, n, j, s->start, s->end + 1);
			out[cnt].type = BP_TYPE_TPOS;
			out[cnt].i = j;
			out[cnt].j = s->start;
			out[cnt].k = s->end + 1;
			cnt++;
			apply_rev(perm, n, j, j + len);
			out[cnt].type = BP_TYPE_REV;
			out[cnt].i = j;
			out[cnt].j = j + len;
			out[cnt].k = -1;
			cnt++;
		}
	}
	return (cnt);
}

/*
 * Lemma 7: move increasing strip S to correct position using ≤4 λ-tpos.
 *
 * R = positions [target, si-1], S = positions [si, s->end].
 * Half-and-half exchange:
 *   Round 1: swap first half of R with first half of S.
 *   Round 2: swap second half of R (if any) with second half of S (if any).
 * Each round needs at most 2 transpositions (one to move, one to re-order).
 * After both rounds, S occupies [target, target+slen-1].
 */
int	move_strip_tpos(int *perm, int n, int lambda, Strip *s, BPOp *out)
{
	int	si;
	int	target;
	int	slen;
	int	rlen;
	int	half;
	int	cnt;
	int	s1;
	int	r1;
	int	pos;
	int	r2;
	int	s2;
	int	s2_start;

	si = s->start;
	target = abs(perm[si]) - 1;
	slen = s->end - si + 1;
	rlen = si - target;
	if (slen == 0 || rlen == 0)
		return (0);
	/* single transposition is enough when total fits in λ */
	if (slen + rlen <= lambda)
	{
		apply_tpos(perm, n, target, si, si + slen);
		out[0].type = BP_TYPE_TPOS;
		out[0].i = target;
		out[0].j = si;
		out[0].k = si + slen;
		return (1);
	}
	half = lambda / 2;
	s1 = (slen > half) ? half : slen;
	r1 = (rlen > half) ? half : rlen;
	cnt = 0;
	pos = target;
	/* Round 1: swap R1=[pos, pos+r1) with S1=[si, si+s1) */
	if (r1 > 0 && s1 > 0 && r1 + s1 <= lambda)
	{
		apply_tpos(perm, n, pos, pos + r1, pos + r1 + s1);
		out[cnt].type = BP_TYPE_TPOS;
		out[cnt].i = pos;
		out[cnt].j = pos + r1;
		out[cnt].k = pos + r1 + s1;
		cnt++;
		pos += s1;
	}
	/* Round 2: swap R2=[pos, pos+rlen-r1) with S2=[si+s1, s->end] */
	{
		r2 = rlen - r1;
		s2 = slen - s1;
		if (r2 > 0 && s2 > 0 && r2 + s2 <= lambda)
		{
			s2_start = pos + r2;
			apply_tpos(perm, n, pos, s2_start, s2_start + s2);
			out[cnt].type = BP_TYPE_TPOS;
			out[cnt].i = pos;
			out[cnt].j = s2_start;
			out[cnt].k = s2_start + s2;
			cnt++;
		}
		else if (r2 == 0 && s2 > 0)
		{
			/* S2 is already in correct segment but may need one more move */
			if (s2 <= lambda)
			{
				apply_tpos(perm, n, pos, pos + s2, pos + s2 + 1 <= n ? pos + s2
					+ 1 : n);
				(void)0;
			}
		}
	}
	return (cnt);
}

/*
 * Lemma 8: move increasing strip S=[si..si+slen-1] to [target..target+slen-1]
 * using λ-reversals.  Each step: reverse [cur_s-move, cur_s+slen-1] (length
 * move+slen ≤ λ) to shift S left by `move`, then reverse [cur_s-move,
 * cur_s-move+slen-1] (length slen) to restore S's ascending order.
 * Returns number of ops applied.
 */
int	move_strip_rev(int *perm, int n, int lambda, Strip *s, BPOp *out)
{
	int	si;
	int	target;
	int	slen;
	int	half;
	int	cnt;
	int	cur_s;
	int	move;

	si = s->start;
	target = abs(perm[si]) - 1;
	slen = s->end - si + 1;
	if (si <= target || slen >= lambda)
		return (0);
	half = lambda / 2;
	if (half > lambda - slen)
		half = lambda - slen;
	if (half <= 0)
		return (0);
	cnt = 0;
	cur_s = si;
	while (cur_s > target && cnt < 5 + lambda - 1)
	{
		move = (cur_s - target < half) ? (cur_s - target) : half;
		if (cur_s - move < 0 || cur_s + slen - 1 >= n)
			break ;
		apply_rev(perm, n, cur_s - move, cur_s + slen - 1);
		out[cnt].type = BP_TYPE_REV;
		out[cnt].i = cur_s - move;
		out[cnt].j = cur_s + slen - 1;
		out[cnt].k = -1;
		cnt++;
		if (slen > 1)
		{
			apply_rev(perm, n, cur_s - move, cur_s - move + slen - 1);
			out[cnt].type = BP_TYPE_REV;
			out[cnt].i = cur_s - move;
			out[cnt].j = cur_s - move + slen - 1;
			out[cnt].k = -1;
			cnt++;
		}
		cur_s -= move;
	}
	return (cnt);
}

/* find the strip containing the smallest out-of-place element */
static int	find_soop_strip(int *perm, int n, int is_signed)
{
	int		sc;
	Strip	*strips;
	int		soop;
	int		res;
	int		idx;

	soop = find_soop(perm, n);
	if (soop < 0)
		return (-1);
	strips = find_strips(perm, n, is_signed, &sc);
	res = -1;
	for (idx = 0; idx < sc; idx++)
	{
		if (strips[idx].start <= soop && soop <= strips[idx].end)
		{
			res = idx;
			break ;
		}
	}
	xfree(strips, sizeof(Strip) * n);
	return (res);
}

/* ── entry points ────────────────────────────────────────────────── */

/*
 * Transposition strip detection: maximal ascending consecutive runs only.
 * (perm[j+1]-perm[j]==1). No decreasing multi-element strips exist.
 * All resulting strips are is_increasing=1 by definition.
 */
static Strip	*find_tpos_strips(int *perm, int n, int *strip_count)
{
	Strip	*strips;
	int		cnt;
	int		j;

	strips = xmalloc(sizeof(Strip) * n);
	cnt = 0;
	for (int i = 0; i < n;)
	{
		strips[cnt].start = i;
		j = i + 1;
		while (j < n && perm[j] - perm[j - 1] == 1)
			j++;
		strips[cnt].end = j - 1;
		strips[cnt].is_increasing = 1;
		i = j;
		cnt++;
	}
	*strip_count = cnt;
	return (strips);
}

BPOp	*lp_bp_tpos(int *perm, int n, int lambda, int *op_count)
{
	int		cap;
	BPOp	*ops;
	int		cnt;
	int		*cur;
	int		sc;
	Strip	*strips;
	int		sidx;
	BPOp	local[8];
	int		napplied;
	int		soop;

	cap = n * 4 + 4;
	ops = xmalloc(sizeof(BPOp) * cap);
	cnt = 0;
	cur = xmalloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	while ((soop = find_soop(cur, n)) >= 0)
	{
		strips = find_tpos_strips(cur, n, &sc);
		sidx = -1;
		for (int idx = 0; idx < sc; idx++)
		{
			if (strips[idx].start <= soop && soop <= strips[idx].end)
			{
				sidx = idx;
				break ;
			}
		}
		if (sidx < 0)
		{
			xfree(strips, sizeof(Strip) * n);
			break ;
		}
		napplied = move_strip_tpos(cur, n, lambda, &strips[sidx], local);
		xfree(strips, sizeof(Strip) * n);
		if (napplied == 0)
			break ;
		for (int k = 0; k < napplied; k++)
			buf_push(&ops, &cnt, &cap, local[k].type, local[k].i, local[k].j,
				local[k].k);
	}
	xfree(cur, sizeof(int) * n);
	ops = xrealloc(ops, sizeof(BPOp) * cap, sizeof(BPOp) * cnt);
	*op_count = cnt;
	return (ops);
}

BPOp	*lp_bp_both(int *perm, int n, int lambda, int is_signed, int *op_count)
{
	int		cap;
	BPOp	*ops;
	int		cnt;
	int		*cur;
	int		sc;
	Strip	*strips;
	int		sidx;
	BPOp	local[8];
	int		napplied;

	cap = n * 4 + 4;
	ops = xmalloc(sizeof(BPOp) * cap);
	cnt = 0;
	cur = xmalloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	while (find_soop(cur, n) >= 0)
	{
		strips = find_strips(cur, n, is_signed, &sc);
		sidx = find_soop_strip(cur, n, is_signed);
		if (sidx < 0 || sidx >= sc)
		{
			xfree(strips, sizeof(Strip) * n);
			break ;
		}
		if (!strips[sidx].is_increasing)
			napplied = move_strip_dec(cur, n, lambda, &strips[sidx], local);
		else
			napplied = move_strip_tpos(cur, n, lambda, &strips[sidx], local);
		xfree(strips, sizeof(Strip) * n);
		if (napplied == 0)
			break ;
		for (int k = 0; k < napplied; k++)
			buf_push(&ops, &cnt, &cap, local[k].type, local[k].i, local[k].j,
				local[k].k);
	}
	xfree(cur, sizeof(int) * n);
	ops = xrealloc(ops, sizeof(BPOp) * cap, sizeof(BPOp) * cnt);
	*op_count = cnt;
	return (ops);
}

BPOp	*lp_bp_rev(int *perm, int n, int lambda, int is_signed, int *op_count)
{
	int		cap;
	BPOp	*ops;
	int		cnt;
	int		*cur;
	int		sc;
	Strip	*strips;
	int		sidx;
	BPOp	local[16];
	int		napplied;

	cap = n * (6 + lambda) + 4;
	ops = xmalloc(sizeof(BPOp) * cap);
	cnt = 0;
	cur = xmalloc(sizeof(int) * n);
	memcpy(cur, perm, sizeof(int) * n);
	/* Lemma 10 pre-pass: reverse all decreasing strips */
	strips = find_strips(cur, n, is_signed, &sc);
	for (int idx = 0; idx < sc; idx++)
	{
		if (!strips[idx].is_increasing)
		{
			apply_rev(cur, n, strips[idx].start, strips[idx].end);
			buf_push(&ops, &cnt, &cap, BP_TYPE_REV, strips[idx].start,
				strips[idx].end, -1);
		}
	}
	xfree(strips, sizeof(Strip) * n);
	while (find_soop(cur, n) >= 0)
	{
		strips = find_strips(cur, n, is_signed, &sc);
		sidx = find_soop_strip(cur, n, is_signed);
		if (sidx < 0 || sidx >= sc)
		{
			xfree(strips, sizeof(Strip) * n);
			break ;
		}
		if (!strips[sidx].is_increasing && strips[sidx].end - strips[sidx].start
			+ 1 <= lambda)
		{
			apply_rev(cur, n, strips[sidx].start, strips[sidx].end);
			buf_push(&ops, &cnt, &cap, BP_TYPE_REV, strips[sidx].start,
				strips[sidx].end, -1);
			xfree(strips, sizeof(Strip) * n);
			continue ;
		}
		napplied = move_strip_rev(cur, n, lambda, &strips[sidx], local);
		xfree(strips, sizeof(Strip) * n);
		if (napplied == 0)
			break ;
		for (int k = 0; k < napplied; k++)
			buf_push(&ops, &cnt, &cap, local[k].type, local[k].i, local[k].j,
				local[k].k);
	}
	xfree(cur, sizeof(int) * n);
	ops = xrealloc(ops, sizeof(BPOp) * cap, sizeof(BPOp) * cnt);
	*op_count = cnt;
	return (ops);
}
