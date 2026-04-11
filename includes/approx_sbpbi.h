/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   approx_sbpbi.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:10:34 by thirata           #+#    #+#             */
/*   Updated: 2026/04/11 22:42:28 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APPROX_SBPBI_H
# define APPROX_SBPBI_H

/*
 * 2-approximation algorithm for sorting by prefix block-interchanges.
 * Based on: Labarre (2018), Algorithm 1 (ApproximateSbpbi).
 *
 * Permutation convention: 0-indexed, values in {0,...,n-1}.
 */

/*
 * Sort perm in-place using prefix block-interchanges.
 *
 * ops: if non-NULL, records each operation as {j, k, l}
 *      (the three arguments passed to prefix_block_interchange).
 *      The caller must ensure ops has space for at least n entries.
 * Returns: the number of operations applied.
 */

# include "lib.h"

typedef struct s_sbpbi_ctx
{
	t_bp_graph	*bp;
	int			size;
	int 		*arr;
	int 		n;
	int			*tmp;
	FILE		*fp;
	int			count;
}				t_sbpbi_ctx;

void	sbpbi_ctx_free(t_sbpbi_ctx *ctx);

void	refresh_tmp(t_sbpbi_ctx *ctx);
int		approx_sbpbi(t_sbpbi_ctx *ctx, int log_output);

int		solve(int *arr, int n, int log_output);

#endif
