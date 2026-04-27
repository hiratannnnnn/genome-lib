#ifndef FOUNDATIONS_H
# define FOUNDATIONS_H

# include "../../includes/memory.h"

/*
 * Permutation convention: 0-indexed positions, values in {1..n} (unsigned)
 * or {-n..-1, 1..n} (signed).
 * perm[i]: element at position i (0-based).
 */

/* ── Operations ──────────────────────────────────────────────────── */

void apply_reversal       (int *perm, int n, int i, int j);
void apply_signed_reversal(int *perm, int n, int i, int j);
void apply_transposition  (int *perm, int n, int i, int j, int k);

/* ── Inversions ──────────────────────────────────────────────────── */

int count_inversions         (int *perm, int n);
int delta_inv_reversal       (int *perm, int n, int i, int j);
int delta_inv_transposition  (int *perm, int n, int i, int j, int k);

/* ── Entropy ─────────────────────────────────────────────────────── */

int entropy_element   (int val, int pos_0indexed);
int entropy_perm      (int *perm, int n);
int count_E_minus_even(int *perm, int n);
int count_E_plus_odd  (int *perm, int n);

/* ── Breakpoints ─────────────────────────────────────────────────── */

int count_breakpoints_unsigned_rev (int *perm, int n);
int count_breakpoints_transposition(int *perm, int n);

/* ── Lambda-permutation ──────────────────────────────────────────── */

int is_lambda_perm           (int *perm, int n, int lambda);
int find_smallest_out_of_place(int *perm, int n);

/* ── Strips ──────────────────────────────────────────────────────── */

typedef struct
{
    int start;
    int end;
    int is_increasing;
} Strip;

Strip *find_strips(int *perm, int n, int is_signed, int *strip_count);

#endif
