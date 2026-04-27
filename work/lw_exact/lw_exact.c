#include "lw_exact.h"
#include "../foundations/foundations.h"
#include "../../includes/memory.h"
#include <math.h>

/*
 * [2020] Exact algorithms for alpha ≥ 3
 *
 * TODO: implement
 *   - Unsigned exact (Theorem 9): remove inversions with size-2 ops
 *   - Signed reversal exact (Theorem 12): inversion removal + sign fix
 *   - Signed both exact (Theorem 13): use Lemma 26 to preserve codd
 */
LWExactOp *lw_exact(int *perm, int n, double alpha, int mode, int *op_count)
{
    (void)perm; (void)n; (void)alpha; (void)mode;
    *op_count = 0;
    return NULL;
}
