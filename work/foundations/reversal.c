#include "foundations.h"
#include <stdlib.h>

/* apply unsigned reversal ρ(i,j) in-place (0-indexed) */
void apply_reversal(int *perm, int n, int i, int j)
{
    (void)n;
    while (i < j)
    {
        int tmp = perm[i];
        perm[i] = perm[j];
        perm[j] = tmp;
        i++;
        j--;
    }
}

/* apply signed reversal ρ̄(i,j): reverse segment and flip signs */
void apply_signed_reversal(int *perm, int n, int i, int j)
{
    (void)n;
    int l = i, r = j;
    while (l < r)
    {
        int tmp = -perm[l];
        perm[l] = -perm[r];
        perm[r] = tmp;
        l++;
        r--;
    }
    if (l == r)
        perm[l] = -perm[l];
}
