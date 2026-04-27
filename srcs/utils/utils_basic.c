/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_basic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:12:24 by thirata           #+#    #+#             */
/*   Updated: 2026/04/10 08:12:25 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

int	sum_matrix(int **matrix, int r, int c)
{
	int	sum;
	int	i;

	sum = 0;
	for (i = 0; i < r; i++)
		sum += sum_array(matrix[i], c);
	return (sum);
}

int	sum_array(int *arr, int n)
{
	int	sum;
	int	j;

	sum = 0;
	for (j = 0; j < n; j++)
		sum += arr[j];
	return (sum);
}

int	min_int(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

int	max_int(int a, int b)
{
	if (a < b)
		return (b);
	return (a);
}

double	min_double(double a, double b)
{
	if (fabs(a - b) < DBL_EPSILON)
		return (a);
	if (a < b)
		return (a);
	return (b);
}

double	max_double(double a, double b)
{
	if (fabs(a - b) < DBL_EPSILON)
		return (a);
	if (a < b)
		return (b);
	return (a);
}
