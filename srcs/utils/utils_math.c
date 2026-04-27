/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_math.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:12:34 by thirata           #+#    #+#             */
/*   Updated: 2026/04/10 22:29:51 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

/**
 * @brief gens n length permutation randly
 * 0, 1, ..., n - 1
 * @param n size of array
 */

int	*rand_perm(int n, int is_natural)
{
	int	*perm;

	int i, j, tmp;
	if (is_natural != 0 && is_natural != 1)
		return (NULL);
	perm = xmalloc(sizeof(int) * n);
	if (!perm)
		return (NULL);
	for (i = 0; i < n; i++)
		perm[i] = i + is_natural;
	for (i = n - 1; i > 0; i--) // Fisher-Yates Shuffle
	{
		j = rand() % (i + 1);
		tmp = perm[i];
		perm[i] = perm[j];
		perm[j] = tmp;
	}
	return (perm);
}
