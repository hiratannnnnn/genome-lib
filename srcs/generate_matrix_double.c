/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_matrix_double.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:11:08 by thirata           #+#    #+#             */
/*   Updated: 2026/04/10 08:11:09 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

double	**gen_matrix_double(int r, int c)
{
	int		i;
	double	**matrix;

	matrix = (double **)xmalloc(sizeof(double *) * r);
	if (!matrix)
		return (NULL);
	for (i = 0; i < r; i++)
	{
		matrix[i] = (double *)xcalloc(c, sizeof(double));
		if (!matrix[i])
		{
			free_matrix_double(matrix, i, c);
			return (NULL);
		}
	}
	return (matrix);
}
