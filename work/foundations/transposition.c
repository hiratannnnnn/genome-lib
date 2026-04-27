#include "foundations.h"
#include "../../includes/memory.h"
#include <string.h>

/* apply transposition τ(i,j,k): swap [i,j) and [j,k) (0-indexed) */
void apply_transposition(int *perm, int n, int i, int j, int k)
{
    (void)n;
    int seg1 = j - i;
    int seg2 = k - j;
    int *tmp = xmalloc(sizeof(int) * (k - i));
    memcpy(tmp,          perm + j, sizeof(int) * seg2);
    memcpy(tmp + seg2,   perm + i, sizeof(int) * seg1);
    memcpy(perm + i, tmp,          sizeof(int) * (seg1 + seg2));
    xfree(tmp, sizeof(int) * (k - i));
}
