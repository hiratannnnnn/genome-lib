#include "foundations.h"
#include <stdlib.h>

/* apply unsigned reversal ρ(i,j) in-place (0-indexed) */
void	apply_rev(int *perm, int n, int i, int j)
{
	int	tmp;

	(void)n;
	while (i < j)
	{
		tmp = perm[i];
		perm[i] = perm[j];
		perm[j] = tmp;
		i++;
		j--;
	}
}

/* apply signed reversal ρ̄(i,j): reverse segment and flip signs */
void	apply_srev(int *perm, int n, int i, int j)
{
	int	l;
	int	r;
	int	tmp;

	(void)n;
	l = i;
	r = j;
	while (l < r)
	{
		tmp = -perm[l];
		perm[l] = -perm[r];
		perm[r] = tmp;
		l++;
		r--;
	}
	if (l == r)
		perm[l] = -perm[l];
}
