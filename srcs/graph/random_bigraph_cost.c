/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_bigraph_cost.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:12:03 by thirata           #+#    #+#             */
/*   Updated: 2026/04/10 08:12:04 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

double	**gen_rand_bigraph_cost(int a, int b)
{
	double	**matrix;

	int i, j;
	matrix = gen_matrix_double(a, b);
	if (!matrix)
		return (NULL);
	for (i = 0; i < a; i++)
		for (j = 0; j < b; j++)
			matrix[i][j] = (double)rand() / RAND_MAX;
	return (matrix);
}
