#ifndef LP_BREAKPOINTS_H
# define LP_BREAKPOINTS_H

/*
 * [2021] Miranda et al. — Theorems 4, 5, 6
 *
 * Theorem 4: (10+2λ)-approx for λ-Reversals  (unsigned & signed)
 * Theorem 5: 12-approx for λ-Transpositions  (unsigned)
 * Theorem 6: 12-approx for λ-Reversals & λ-Transpositions (unsigned & signed)
 *
 * Strategy: repeatedly move the strip containing the smallest
 * out-of-place element to its correct position, reducing b(π) by ≥ 1.
 *
 * Key lemmas implemented here:
 *   Lemma 7  — move increasing strip with ≤4 λ-transpositions
 *   Lemma 8  — move increasing strip with ≤(5+λ-1) λ-reversals
 *   Lemma 9  — handle decreasing strip with ≤2 λ-operations
 *   Lemma 10 — reverse a decreasing strip → λ-permutation preserved
 */

# include "../foundations/foundations.h"

# define BP_TYPE_REV 1
# define BP_TYPE_TPOS 2

typedef struct
{
	int	type;
	int	i;
	int	j;
	int	k;
}		BPOp;

/* entry points */
BPOp	*lp_bp_rev(int *perm, int n, int lambda, int is_signed, int *op_count);
BPOp	*lp_bp_tpos(int *perm, int n, int lambda, int *op_count);
BPOp	*lp_bp_both(int *perm, int n, int lambda, int is_signed, int *op_count);

/* internal lemma helpers (also usable standalone) */
int		move_strip_tpos(int *perm, int n, int lambda, Strip *s, BPOp *out);
int		move_strip_rev(int *perm, int n, int lambda, Strip *s, BPOp *out);
int		move_strip_dec(int *perm, int n, int lambda, Strip *s, BPOp *out);
int		rev_dec_strip(int *perm, int n, int lambda, Strip *s);

#endif
