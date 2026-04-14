/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 22:22:53 by thirata           #+#    #+#             */
/*   Updated: 2026/04/14 14:30:58 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "approx_sbpbi.h"

void	sbpbi_ctx_free(t_sbpbi_ctx *ctx)
{
	xfree(ctx->bp, sizeof(t_bp_graph) * ctx->size);
	free_array_int(ctx->tmp, ctx->size);
	fclose(ctx->fp);
}

static int	sbpbi_ctx_init(t_sbpbi_ctx *ctx, int *arr, int n)
{
	ctx->size = 2 * (n + 1);
	ctx->bp = (t_bp_graph *)xmalloc(sizeof(t_bp_graph) * ctx->size);
	ctx->tmp = (int *)xmalloc(sizeof(int) * ctx->size);
	if (!ctx->bp || !ctx->tmp)
		return (sbpbi_ctx_free(ctx), 0);
	ctx->fp = fopen("log", "w");
	if (ctx->fp == NULL)
		return (sbpbi_ctx_free(ctx), 0);
	bp_graph_init(ctx->bp, ctx->size, arr, n);
	ctx->arr = arr;
	ctx->n = n;
	ctx->count = 0;
	return (1);
}

int	solve(int *arr, int n, int log_output)
{
	t_sbpbi_ctx	ctx;
	int			result;

	if (!sbpbi_ctx_init(&ctx, arr, n) || !(log_output == 1 || log_output == 0))
		return (-1);
	result = approx_sbpbi(&ctx, log_output);
	// printf("finished.\n");
	sbpbi_ctx_free(&ctx);
	return (result);
}
