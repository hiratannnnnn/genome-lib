/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 00:13:31 by thirata           #+#    #+#             */
/*   Updated: 2026/04/12 00:24:45 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

int main(int ac, char **av)
{
	int n;
	int *arr;
	FILE *fp;

	fp = fopen("input", "w");
	if (ac != 2)
		return (fclose(fp), 1);
	n = atoi(av[1]);
	arr = rand_perm(n, 1);
	if (!arr)
		return (1);
	fprint_array_int(fp, arr, n, 0);

	fclose(fp);
	free_array_int(arr, n);

	return (0);
}
