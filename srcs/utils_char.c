#include "lib.h"

void 	free_array_char(char **ss, int n)
{
	int i;

	i = 0;
	while (i < n && ss[i])
	{
		xfree(ss[i], sizeof(char) * (strlen(ss[i]) + 1));
		i++;
	}
	xfree(ss, sizeof(char *) * n);
}

int 	count_digit(int number)
{
	if (number == -2147483648)
		return 11;
	if (number < 0)
		return (count_digit((-1) * number) + 1);
	if (number < 10)
		return (1);
	return (count_digit(number / 10) + 1);
}
