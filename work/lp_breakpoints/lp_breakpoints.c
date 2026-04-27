#include "../../includes/memory.h"
#include "lp_breakpoints.h"

/*
 * [2021] Theorems 4, 5, 6 — breakpoints-based algorithms
 *
 * All three entry points share the same outer loop:
 *   while (b(π) > 0):
 *     find smallest out-of-place element → find its strip S
 *     move S to its correct position using Lemma 7, 8, or 9
 *
 * TODO: implement lemma helpers and entry points
 */

int	move_strip_tpos(int *perm, int n, int lambda, Strip *s, BPOp *out)
{
	(void)perm;
	(void)n;
	(void)lambda;
	(void)s;
	(void)out;
	/* Lemma 7: ≤4 λ-transpositions */
	return (0);
}

int	move_strip_rev(int *perm, int n, int lambda, Strip *s, BPOp *out)
{
	(void)perm;
	(void)n;
	(void)lambda;
	(void)s;
	(void)out;
	/* Lemma 8: ≤(5+λ-1) λ-reversals */
	return (0);
}

int	move_strip_dec(int *perm, int n, int lambda, Strip *s, BPOp *out)
{
	(void)perm;
	(void)n;
	(void)lambda;
	(void)s;
	(void)out;
	/* Lemma 9: ≤1 λ-transposition + ≤1 λ-reversal */
	return (0);
}

int	rev_dec_strip(int *perm, int n, int lambda, Strip *s)
{
	(void)perm;
	(void)n;
	(void)lambda;
	(void)s;
	/* Lemma 10: reverse the strip, verify λ-perm is preserved */
	return (0);
}

BPOp	*lp_bp_rev(int *perm, int n, int lambda, int is_signed, int *op_count)
{
	(void)perm;
	(void)n;
	(void)lambda;
	(void)is_signed;
	*op_count = 0;
	return (NULL);
}

BPOp	*lp_bp_tpos(int *perm, int n, int lambda, int *op_count)
{
	(void)perm;
	(void)n;
	(void)lambda;
	*op_count = 0;
	return (NULL);
}

BPOp	*lp_bp_both(int *perm, int n, int lambda, int is_signed, int *op_count)
{
	(void)perm;
	(void)n;
	(void)lambda;
	(void)is_signed;
	*op_count = 0;
	return (NULL);
}
