#ifndef LW_EXACT_H
# define LW_EXACT_H

/*
 * [2020] Exact algorithms for alpha ≥ 3
 *
 * Theorem 9  — SbR, SbT, SbRT (unsigned): polynomial exact solution
 * Theorem 12 — SbR̄ (signed reversals): polynomial exact solution
 * Theorem 13 — SbR̄T (signed reversals & transpositions): polynomial exact
 *
 * Key insight: for alpha ≥ 3, any large operation can be replaced by a
 * sequence of super-short (size ≤ 2) operations with no greater total cost
 * (Lemmas 23, 24). The optimal sorting sequence uses only super-short ops.
 *
 * For unsigned (Theorem 9):
 *   Optimal cost = 2^alpha * Inv(π)
 *   Algorithm: apply size-2 reversal or transposition to remove each inversion.
 *
 * For signed reversals (Theorem 12):
 *   Optimal cost = 2^alpha * Inv(π) + |E^-_even| + |E^+_odd|
 *   Algorithm: (i) remove all inversions with size-2 signed reversals;
 *              (ii) fix signs with size-1 reversals.
 *
 * For signed reversals & transpositions (Theorem 13):
 *   Optimal cost = 2^alpha * Inv(π) + codd(G(π))
 *   Algorithm: (i) remove inversions without increasing codd (Lemma 26);
 *              (ii) fix one odd component with size-1 signed reversal.
 *
 * mode: 1=SbR, 2=SbT, 3=SbRT, 4=SbRbar, 5=SbRbarT
 */

typedef struct
{
	int		type;
	int		i;
	int		j;
	int		k;
}			LWExactOp;

LWExactOp	*lw_exact(int *perm, int n, double alpha, int mode, int *op_count);

#endif
