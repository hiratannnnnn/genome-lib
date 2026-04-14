/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_permutation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 21:00:59 by thirata           #+#    #+#             */
/*   Updated: 2026/04/11 21:15:51 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

static void	swap_int(int *a, int *b)
{
	int	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	rev_array_int(int *begin, int *end)
{
	while (begin < end)
	{
		swap_int(begin++, end--);
	}
}

int	next_permutation(int *arr, int n)
{
	int	i;
	int	j;

	i = n - 2;
	while (i >= 0 && arr[i] >= arr[i + 1])
	{
		i--;
	}
	if (i < 0)
		return (0);
	j = n - 1;
	while (arr[i] >= arr[j])
	{
		j--;
	}
	swap_int(&arr[i], &arr[j]);
	rev_array_int(&arr[i + 1], &arr[n - 1]);
	return (1);
}
