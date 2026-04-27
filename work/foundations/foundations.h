#ifndef FOUNDATIONS_H
# define FOUNDATIONS_H

# include "../../includes/memory.h"

/*
 * Permutation convention: 0-indexed positions, values in {1..n} (unsigned)
 * or {-n..-1, 1..n} (signed).
 * perm[i]: element at position i (0-based).
 * See work/ABBREV.md for the full abbreviation reference.
 */

/* ── Operations ──────────────────────────────────────────────────── */

void	apply_rev(int *perm, int n, int i, int j);
void	apply_srev(int *perm, int n, int i, int j);
void	apply_tpos(int *perm, int n, int i, int j, int k);

/* ── Inversions ──────────────────────────────────────────────────── */

int		cnt_inv(int *perm, int n);
int		delta_inv_rev(int *perm, int n, int i, int j);
int		delta_inv_tpos(int *perm, int n, int i, int j, int k);

/* ── Inversion graph ─────────────────────────────────────────────── */

int		codd_perm(int *perm, int n);

/* ── Entropy ─────────────────────────────────────────────────────── */

int		ent_elem(int val, int pos_0indexed);
int		ent_perm(int *perm, int n);
int		cnt_Eminus(int *perm, int n);
int		cnt_Eplus(int *perm, int n);

/* ── Breakpoints ─────────────────────────────────────────────────── */

int		cnt_bp_rev(int *perm, int n);
int		cnt_bp_tpos(int *perm, int n);

/* ── Lambda-permutation ──────────────────────────────────────────── */

int		is_lam_perm(int *perm, int n, int lambda);
int		find_soop(int *perm, int n);

/* ── Strips ──────────────────────────────────────────────────────── */

typedef struct
{
	int	start;
	int	end;
	int	is_increasing;
}		Strip;

Strip	*find_strips(int *perm, int n, int is_signed, int *strip_count);

#endif
