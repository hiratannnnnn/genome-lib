#include "foundations.h"
#include <stdlib.h>

/* ── Inversions ─────────────────────────────────────────────────── */

int	cnt_inv(int *perm, int n)
{
	int	inv;

	inv = 0;
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			if (abs(perm[i]) > abs(perm[j]))
				inv++;
	return (inv);
}

/* change in Inv caused by unsigned reversal ρ(i,j) (0-indexed) */
int	delta_inv_rev(int *perm, int n, int i, int j)
{
	int	delta;
	int	total;
	int	seg_len;
	int	max_inv;

	(void)n;
	delta = 0;
	for (int a = i; a <= j; a++)
		for (int b = a + 1; b <= j; b++)
			if (abs(perm[a]) > abs(perm[b]))
				delta++; /* inversion inside segment — survives reversal */
	/* total inversions in segment before reversal */
	total = 0;
	for (int a = i; a < j; a++)
		for (int b = a + 1; b <= j; b++)
			if (abs(perm[a]) > abs(perm[b]))
				total++;
	seg_len = j - i + 1;
	max_inv = seg_len * (seg_len - 1) / 2;
	/* after reversal, inversions inside become (max_inv - total) */
	return (total - (max_inv - total));
}

int	delta_inv_tpos(int *perm, int n, int i, int j, int k)
{
	int	removed;
	int	added;

	(void)n;
	removed = 0;
	for (int a = i; a < j; a++)
		for (int b = j; b < k; b++)
			if (abs(perm[a]) > abs(perm[b]))
				removed++;
	added = 0;
	for (int a = j; a < k; a++)
		for (int b = i; b < j; b++)
			if (abs(perm[a]) > abs(perm[b]))
				added++;
	return (removed - added);
}

/* ── Entropy ─────────────────────────────────────────────────────── */

/* ent(πi) = ||perm[i]| - (i+1)|  (position is 1-indexed in paper) */
int	ent_elem(int val, int pos_0indexed)
{
	int	diff;

	diff = abs(val) - (pos_0indexed + 1);
	return (diff < 0 ? -diff : diff);
}

int	ent_perm(int *perm, int n)
{
	int	e;

	e = 0;
	for (int i = 0; i < n; i++)
		e += ent_elem(perm[i], i);
	return (e);
}

/* E^-_even: negative elements with even entropy */
int	cnt_Eminus(int *perm, int n)
{
	int	cnt;

	cnt = 0;
	for (int i = 0; i < n; i++)
		if (perm[i] < 0 && ent_elem(perm[i], i) % 2 == 0)
			cnt++;
	return (cnt);
}

/* E^+_odd: positive elements with odd entropy */
int	cnt_Eplus(int *perm, int n)
{
	int	cnt;

	cnt = 0;
	for (int i = 0; i < n; i++)
		if (perm[i] > 0 && ent_elem(perm[i], i) % 2 == 1)
			cnt++;
	return (cnt);
}

/* ── Inversion graph ─────────────────────────────────────────────── */

/*
 * Build the inversion graph G(|π|) on vertices {1..n}:
 * edge {a,b} exists iff elements a and b form an inversion in |π|.
 * Return the number of odd-size connected components (codd).
 * Used in the ψ-score: ψ(π) = 2·Inv(|π|) + codd(G(|π|)).
 */
static int	uf_find(int *parent, int x)
{
	while (parent[x] != x)
		x = parent[x];
	return (x);
}

static void	uf_union(int *parent, int *sz, int x, int y)
{
	x = uf_find(parent, x);
	y = uf_find(parent, y);
	if (x == y)
		return ;
	if (sz[x] < sz[y])
	{
		int	t;

		t = x;
		x = y;
		y = t;
	}
	parent[y] = x;
	sz[x] += sz[y];
}

int	codd_perm(int *perm, int n)
{
	int	*parent;
	int	*sz;
	int	codd;

	parent = xmalloc(sizeof(int) * (n + 1));
	sz = xmalloc(sizeof(int) * (n + 1));
	for (int i = 1; i <= n; i++)
	{
		parent[i] = i;
		sz[i] = 1;
	}
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			if (abs(perm[i]) > abs(perm[j]))
				uf_union(parent, sz, abs(perm[i]), abs(perm[j]));
	codd = 0;
	for (int v = 1; v <= n; v++)
		if (parent[v] == v && sz[v] % 2 == 1)
			codd++;
	xfree(parent, sizeof(int) * (n + 1));
	xfree(sz, sizeof(int) * (n + 1));
	return (codd);
}

/* ── Breakpoints ─────────────────────────────────────────────────── */

/* unsigned reversal breakpoint: |perm[i+1] - perm[i]| != 1
 * includes sentinel: perm[-1]=0, perm[n]=n+1 (1-indexed values) */
int	cnt_bp_rev(int *perm, int n)
{
	int	bp;
	int	prev;
	int	diff;

	bp = 0;
	prev = 0;
	for (int i = 0; i < n; i++)
	{
		diff = abs(perm[i]) - prev;
		if (diff != 1 && diff != -1)
			bp++;
		prev = abs(perm[i]);
	}
	if (abs(perm[n - 1]) != n)
		bp++;
	return (bp);
}

/* transposition / signed reversal breakpoint: perm[i+1] - perm[i] != 1 */
int	cnt_bp_tpos(int *perm, int n)
{
	int	bp;

	bp = 0;
	if (perm[0] != 1)
		bp++;
	for (int i = 0; i + 1 < n; i++)
		if (perm[i + 1] - perm[i] != 1)
			bp++;
	if (perm[n - 1] != n)
		bp++;
	return (bp);
}

/* ── Lambda-permutation ──────────────────────────────────────────── */

/* λ-permutation: ||perm[i]| - (i+1)| < lambda for all i (0-indexed) */
int	is_lam_perm(int *perm, int n, int lambda)
{
	int	diff;

	for (int i = 0; i < n; i++)
	{
		diff = abs(perm[i]) - (i + 1);
		if (diff < 0)
			diff = -diff;
		if (diff >= lambda)
			return (0);
	}
	return (1);
}

/* smallest element out-of-place (returns 0-indexed position, or
	-1 if sorted) */
int	find_soop(int *perm, int n)
{
	for (int v = 1; v <= n; v++)
	{
		for (int i = 0; i < n; i++)
		{
			if (abs(perm[i]) == v && i + 1 != v)
				return (i);
		}
	}
	return (-1);
}

/* ── Strips ──────────────────────────────────────────────────────── */

Strip	*find_strips(int *perm, int n, int is_signed, int *strip_count)
{
	Strip	*strips;
	int		cnt;
	int		start;
	int		is_inc;
	int		j;
	int		asc;

	strips = xmalloc(sizeof(Strip) * n);
	cnt = 0;
	start = 0;
	for (int i = 0; i < n;)
	{
		strips[cnt].start = i;
		if (is_signed)
		{
			is_inc = (perm[i] > 0);
			j = i + 1;
			while (j < n && (perm[j] > 0) == is_inc && perm[j] - perm[j
				- 1] == 1)
				j++;
			strips[cnt].end = j - 1;
			strips[cnt].is_increasing = is_inc;
		}
		else
		{
			j = i + 1;
			asc = (j < n && perm[j] - perm[j - 1] == 1);
			while (j < n && ((asc && perm[j] - perm[j - 1] == 1) || (!asc
						&& perm[j - 1] - perm[j] == 1)))
				j++;
			strips[cnt].end = j - 1;
			strips[cnt].is_increasing = asc
				|| (strips[cnt].end == strips[cnt].start);
		}
		i = strips[cnt].end + 1;
		cnt++;
	}
	(void)start;
	*strip_count = cnt;
	return (strips);
}
