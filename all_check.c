/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 23:22:07 by thirata           #+#    #+#             */
/*   Updated: 2026/04/12 00:11:23 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "approx_sbpbi.h"

static void	put_error(void)
{
	printf("[Usage] ./all_check <n> <log_output:0/1>\n");
}

static void	init_free(int *arr, int *copy, int *max_perm, int n)
{
	free_array_int(arr, n);
	free_array_int(copy, n);
	free_array_int(max_perm, n);
}

static int init(int **arr, int **copy, int **max_perm, int n)
{
	*arr = (int *)xmalloc(sizeof(int) * n);
	*copy = (int *)xmalloc(sizeof(int) * n);
	*max_perm = (int *)xmalloc(sizeof(int) * n);
	if (!(*arr) || !(*copy) || !(*max_perm))
		return (init_free(*arr, *copy, *max_perm, n), 0);
	identity_permutation(*arr, n, 1);
	copy_permutation(*arr, *copy, n);
	return (1);
}

int main(int ac, char **av)
{
	int	n, result, res, max;
	int	*arr, *copy, *max_perm;

	if (ac <= 1)
		return (put_error(), 1);
	n = atoi(av[1]);
	if (n <= 0)
		return (put_error(), 1);
	if (!init(&arr, &copy, &max_perm, n))
		return (put_error(), 1);
	max = 0;
	res = 1;
	while (res)
	{
		result = solve(copy, n, 0);
		if (result > max)
		{
			max = result;
			copy_permutation(arr, max_perm, n);
		}
		res = next_permutation(arr, n);
		copy_permutation(arr, copy, n);
	}
	printf("MAX: %d\n", max);
	print_array_int(max_perm, n, 2);
	init_free(arr, copy, max_perm, n);
	print_info();
	return (0);
}
