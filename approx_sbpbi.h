/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   approx_sbpbi.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:10:34 by thirata           #+#    #+#             */
/*   Updated: 2026/04/10 08:10:36 by thirata          ###   ########.fr       */
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
int approx_sbpbi(int *perm, int n, int (*ops)[3]);

#endif
