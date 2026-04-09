/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 20:39:45 by thirata           #+#    #+#             */
/*   Updated: 2026/04/09 20:42:21 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

int main(void)
{
	int n;

	n = 5;
	int *arr;

	arr = (int *)xmalloc(sizeof(int) * 5);
	if (!arr)
		return (1);
	identity_permutation(arr, n);
	print_array_int(arr, n, 0);



	return (0);
}
