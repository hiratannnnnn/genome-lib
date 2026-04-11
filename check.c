/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 23:44:35 by thirata           #+#    #+#             */
/*   Updated: 2026/04/12 00:02:54 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "approx_sbpbi.h"

static int is_distinct(int *arr, int n)
{
	int i;
	int j;
	int tmp;
	int *tmpp;

	tmpp = (int *)xmalloc(sizeof(int) * n);
	if (!tmpp)
		return (0);
	copy_permutation(arr, tmpp, n);
	i = 0;
	while (i < n)
	{
		j = 0;
		while (j < n - i - 1)
		{
			if (tmpp[j] > tmpp[j + 1])
			{
				tmp = tmpp[j];
				tmpp[j] = tmpp[j + 1];
				tmpp[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
	tmp = is_identity_permutation(tmpp, n, 1);
	free_array_int(tmpp, n);
	return (tmp);
}

int main(int ac, char **av)
{
	int	n;
	int	*arr;

	n = ac - 1;
	arr = (int *)xmalloc(sizeof(int) * n);
	if (!arr)
		return (1);

	for (int i = 0; i < n; i++)
		arr[i] = atoi(av[i + 1]);
	print_array_int(arr, n, 0);
	if (!is_distinct(arr, n))
	{
		printf("permutation is not distinct.\n");
		printf("[Usage] ./check 5 3 2 4 1\n");
		return (free_array_int(arr, n), 1);
	}
	solve(arr, n, 1);
	free_array_int(arr, n);
	print_info();
	return (0);
}
