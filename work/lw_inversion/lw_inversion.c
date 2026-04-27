#include "lw_inversion.h"
#include "../foundations/foundations.h"
#include "../../includes/memory.h"
#include <math.h>

/*
 * [2020] Algorithm 1
 * while Inv(π) > 0:
 *   pick β in M_lambda maximising ΔInv(π,β) / |β|^alpha
 *   apply β to π
 *
 * TODO: implement
 */
LWOp *lw_inversion_greedy(int *perm, int n, int lambda, double alpha,
                            int mode, int *op_count)
{
    (void)perm; (void)n; (void)lambda; (void)alpha; (void)mode;
    *op_count = 0;
    return NULL;
}
