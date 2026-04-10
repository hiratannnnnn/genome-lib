/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 22:22:53 by thirata           #+#    #+#             */
/*   Updated: 2026/04/10 23:00:03 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

void solve(int *arr, int n)
{
	print_array_int(arr, n, 0);
	t_bp_graph *bp;
	int size = 2 * (n + 1);
	bp = (t_bp_graph *)xmalloc(sizeof(t_bp_graph) * size);
	if (!bp)
		return ;
	bp_graph_init(bp, size, arr, n);
	print_bp_graph(bp, size);
	xfree(bp, sizeof(t_bp_graph) * size);
}
