/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   benchmark.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 00:00:00 by thirata           #+#    #+#             */
/*   Updated: 2026/04/23 00:00:00 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "approx_sbpbi.h"
#include <time.h>

static double	elapsed_ms(struct timespec *start, struct timespec *end)
{
	return ((end->tv_sec - start->tv_sec) * 1000.0
		+ (end->tv_nsec - start->tv_nsec) / 1e6);
}

static void	put_usage(void)
{
	printf("[Usage] ./benchmark <n> <trials>\n");
	printf("  n      : permutation size\n");
	printf("  trials : number of random permutations to test\n");
}

int	main(int ac, char **av)
{
	int				n;
	int				trials;
	int				*perm;
	int				*copy;
	int				ops;
	int				max_ops;
	int				min_ops;
	long long		total_ops;
	double			elapsed;
	double			total_ms;
	double			min_ms;
	double			max_ms;
	struct timespec	t0;
	struct timespec	t1;
	int				i;

	if (ac != 3)
		return (put_usage(), 1);
	n = atoi(av[1]);
	trials = atoi(av[2]);
	if (n <= 0 || trials <= 0)
		return (put_usage(), 1);
	copy = (int *)xmalloc(sizeof(int) * n);
	if (!copy)
		return (1);
	total_ms = 0.0;
	min_ms = -1.0;
	max_ms = 0.0;
	total_ops = 0;
	max_ops = 0;
	min_ops = -1;
	printf("benchmark: n=%d, trials=%d\n", n, trials);
	printf("-------------------------------\n");
	i = 0;
	while (i < trials)
	{
		perm = rand_perm(n, 1);
		if (!perm)
			return (free_array_int(copy, n), 1);
		copy_permutation(perm, copy, n);
		free_array_int(perm, n);
		clock_gettime(CLOCK_MONOTONIC, &t0);
		ops = solve(copy, n, 0);
		clock_gettime(CLOCK_MONOTONIC, &t1);
		if (ops < 0)
		{
			printf("solve() failed at trial %d\n", i + 1);
			return (free_array_int(copy, n), 1);
		}
		elapsed = elapsed_ms(&t0, &t1);
		total_ms += elapsed;
		if (min_ms < 0 || elapsed < min_ms)
			min_ms = elapsed;
		if (elapsed > max_ms)
			max_ms = elapsed;
		total_ops += ops;
		if (min_ops < 0 || ops < min_ops)
			min_ops = ops;
		if (ops > max_ops)
			max_ops = ops;
		i++;
	}
	free_array_int(copy, n);
	printf("total   time : %.3f ms\n", total_ms);
	printf("average time : %.6f ms\n", total_ms / trials);
	printf("min     time : %.6f ms\n", min_ms);
	printf("max     time : %.6f ms\n", max_ms);
	printf("-------------------------------\n");
	printf("average ops  : %.2f\n", (double)total_ops / trials);
	printf("min     ops  : %d\n", min_ops);
	printf("max     ops  : %d\n", max_ops);
	return (0);
}
