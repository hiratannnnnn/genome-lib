/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   approx_sbpbi.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:10:37 by thirata           #+#    #+#             */
/*   Updated: 2026/04/11 23:11:57 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "approx_sbpbi.h"

/**
 * 1-indexed
 */
typedef struct s_bi_pos
{
	int i;
	int j;
	int k;
	int l;
}	t_bi_pos;

static void	print_pos(t_bi_pos *pos)
{
	printf("bi(%d, %d, %d, %d)\n", pos->i, pos->j, pos->k, pos->l);
}

static void write_to_log(t_sbpbi_ctx *ctx, t_bi_pos *pos)
{
	fprintf(ctx->fp, "[%-5d]:\tBI(%d, %d, %d, %d)\n",
			ctx->count, pos->i, pos->j, pos->k, pos->l);
}

void refresh_tmp(t_sbpbi_ctx *ctx)
{
	int i;
	int index;

	ctx->tmp[0] = 0;
	ctx->tmp[ctx->size - 1] = ctx->size - 1;
	i = 0;
	index = 1;
	while (i < ctx->n)
	{
		index = 2 * i + 1;
		ctx->tmp[index++] = ctx->arr[i] * 2 - 1;
		ctx->tmp[index++] = ctx->arr[i] * 2;
		i++;
	}
}

void block_interchange(t_sbpbi_ctx *ctx, t_bi_pos *pos)
{
	const int i = pos->i;
	const int j = pos->j;
	const int k = pos->k;
	const int l = pos->l;

	if (!(1 <= i && i < j && j <= k && k < l && l <= ctx->n + 1))
	{
		print_pos(pos);
		print_array_int(ctx->arr, ctx->n, 0);
		printf("cannot block interchange.\n");
		sbpbi_ctx_free(ctx);
		exit(1);
	}

	int arr_num;

	ctx->tmp[0] = 0;
	ctx->tmp[ctx->size - 1] = ctx->size - 1;
	int ind;

	ind = 0;
	while (ind < ctx->n)
	{
		arr_num = ctx->arr[ind];
		ctx->tmp[ind * 2 + 1] = arr_num * 2 - 1;
		ctx->tmp[ind * 2 + 2] = arr_num * 2;
		ind++;
	}
	if (j != k)
	{

		/* A: right(pos i-1) <-> left(pos k) */
		ctx->bp[ctx->tmp[2 * (i - 1)]].black = ctx->tmp[2 * k - 1];
		ctx->bp[ctx->tmp[2 * k - 1]].black   = ctx->tmp[2 * (i - 1)];
		/* B: right(pos l-1) <-> left(pos j) */
		ctx->bp[ctx->tmp[2 * (l - 1)]].black = ctx->tmp[2 * j - 1];
		ctx->bp[ctx->tmp[2 * j - 1]].black   = ctx->tmp[2 * (l - 1)];
		/* C: right(pos k-1) <-> left(pos i) */
		ctx->bp[ctx->tmp[2 * (k - 1)]].black = ctx->tmp[2 * i - 1];
		ctx->bp[ctx->tmp[2 * i - 1]].black   = ctx->tmp[2 * (k - 1)];
		/* D: right(pos j-1) <-> left(pos l) */
		ctx->bp[ctx->tmp[2 * (j - 1)]].black = ctx->tmp[2 * l - 1];
		ctx->bp[ctx->tmp[2 * l - 1]].black   = ctx->tmp[2 * (j - 1)];
	}
	else
	{
		/* A: right(pos i-1) <-> left(pos k) */
		ctx->bp[ctx->tmp[2 * (i - 1)]].black = ctx->tmp[2 * k - 1];
		ctx->bp[ctx->tmp[2 * k - 1]].black   = ctx->tmp[2 * (i - 1)];	// k or j
		/* B: right(pos l-1) <-> left(pos j) */
		ctx->bp[ctx->tmp[2 * (l - 1)]].black = ctx->tmp[2 * i - 1];		// j -> i
		// ctx->bp[ctx->tmp[2 * j - 1]].black   = ctx->tmp[2 * (l - 1)];
		/* C: right(pos k-1) <-> left(pos i) */
		// ctx->bp[ctx->tmp[2 * (k - 1)]].black = ctx->tmp[2 * l - 1];
		ctx->bp[ctx->tmp[2 * i - 1]].black   = ctx->tmp[2 * (l - 1)];	// k -> l
		/* D: right(pos j-1) <-> left(pos l) */
		ctx->bp[ctx->tmp[2 * (j - 1)]].black = ctx->tmp[2 * l - 1];		// j or k
		ctx->bp[ctx->tmp[2 * l - 1]].black   = ctx->tmp[2 * (j - 1)];
	}
	/* arr swap: block2=[k..l), middle=[j..k), block1=[i..j)  (1-indexed, exclusive end) */
	memcpy(ctx->tmp,                       ctx->arr + k - 1, sizeof(int) * (l - k));
	memcpy(ctx->tmp + (l - k),             ctx->arr + j - 1, sizeof(int) * (k - j));
	memcpy(ctx->tmp + (l - k) + (k - j),  ctx->arr + i - 1, sizeof(int) * (j - i));
	memcpy(ctx->arr + i - 1, ctx->tmp, sizeof(int) * (l - i));

	ctx->count++;
}

static void set_pos(t_bi_pos *pos, int i, int j, int k, int l)
{
	pos->i = i;
	pos->j = j;
	pos->k = k;
	pos->l = l;
}

int		find_pattern_a(t_sbpbi_ctx *ctx, t_bi_pos *pos)
{
	// printf("[A]\n");
	int first;
	int i;
	int j;
	int k;

	first = ctx->tmp[1];
	j = 0;
	while (j < ctx->size)
	{
		if (ctx->tmp[j] == first - 1)
			break;
		j++;
	}
	// printf("j: %d\n", j);
	i = 2;
	while (i < j)
	{
		k = j + 1;
		while (k < ctx->size)
		{
			if (ctx->tmp[i] + 1 == ctx->tmp[k] &&
				ctx->bp[ctx->tmp[i]].cycle_id == 0)
			{
				//case 2, 3, 4;
				// printf("%d, %d, %d\n", i, j, k);
				if (j + 1 == k)
					set_pos(pos, 1, i / 2 + 1, i / 2 + 1, k / 2 + 1);
				else
					set_pos(pos, 1, i / 2 + 1, j / 2 + 1, k / 2 + 1);
				return (1);
			}
			k++;
		}
		i++;
	}
	return (0);
}

void	find_pattern_b(t_sbpbi_ctx *ctx, t_bi_pos *pos)
{
	// printf("[B]\n");
	int first;
	int i;
	int j;
	int k;

	first = ctx->tmp[1];
	// printf("first: %d\n", first);
	j = 0;
	while (j < ctx->size)
	{
		if (ctx->tmp[j] == first - 1)
			break;
		j++;
	}
	i = 2;
	while (i < j)
	{
		k = j;
		while (k < ctx->size)
		{
			if (ctx->tmp[i] - 1 == ctx->tmp[k])
			{
				// case 1, 4;
				// printf("%d, %d, %d\n", i, j, k);
				set_pos(pos, 1, i / 2 + 1, j / 2 + 1, k / 2 + 1);
				return ;
			}
			k++;
		}
		i++;
	}
}

void	find_pattern_c(t_sbpbi_ctx *ctx, t_bi_pos *pos)
{
	// printf("[C]\n");
	int i;
	int target;

	i = 0;
	while(i < ctx->n)
	{
		if (ctx->arr[i + 1] != ctx->arr[i] + 1)
		{
			target = ctx->arr[i] + 1;
			break;
		}
		i++;
	}
	int j;

	j = i;
	while (j < ctx->n)
	{
		if (ctx->arr[j] == target)
		{
			set_pos(pos, 1, i + 2, i + 2, j + 1);
			return ;
		}
		j++;
	}

}

int approx_sbpbi(t_sbpbi_ctx *ctx, int log_output)
{
	t_bi_pos pos;
	int o_count;

	if (log_output)
	{
		fprintf(ctx->fp, "PERM:\t\t");
		fprint_array_int(ctx->fp, ctx->arr, ctx->n, 2);
	}
	o_count = set_cycle_id(ctx->bp, ctx->size);
	while (o_count < ctx->n + 1)
	{
		refresh_tmp(ctx);
		if (ctx->arr[0] != 1)
		{
			if (find_pattern_a(ctx, &pos))
			{
				// print_pos(&pos);
				block_interchange(ctx, &pos);
				if (log_output)
					write_to_log(ctx, &pos);
			}
			else
			{
				find_pattern_b(ctx, &pos);
				// print_pos(&pos);
				block_interchange(ctx, &pos);
				if (log_output)
					write_to_log(ctx, &pos);
			}
		}
		else
		{
			find_pattern_c(ctx, &pos);
			// print_pos(&pos);
			block_interchange(ctx, &pos);
			if (log_output)
				write_to_log(ctx, &pos);
		}
		// set_pos(&pos, 1, 3, 4, 6);
		// block_interchange(ctx, &pos);
		o_count = set_cycle_id(ctx->bp, ctx->size);
		// usleep(100000);
	}
	return (ctx->count);
}
