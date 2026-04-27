#include "../../includes/memory.h"
#include "foundations.h"
#include <string.h>

/* apply transposition τ(i,j,k): swap [i,j) and [j,k) (0-indexed) */
void	apply_tpos(int *perm, int n, int i, int j, int k)
{
	int	seg1;
	int	seg2;
	int	*tmp;

	(void)n;
	seg1 = j - i;
	seg2 = k - j;
	tmp = xmalloc(sizeof(int) * (k - i));
	memcpy(tmp, perm + j, sizeof(int) * seg2);
	memcpy(tmp + seg2, perm + i, sizeof(int) * seg1);
	memcpy(perm + i, tmp, sizeof(int) * (seg1 + seg2));
	xfree(tmp, sizeof(int) * (k - i));
}
