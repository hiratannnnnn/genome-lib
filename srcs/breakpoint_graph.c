/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   breakpoint_graph.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:10:43 by thirata           #+#    #+#             */
/*   Updated: 2026/04/11 00:03:34 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

void print_bp_graph(t_bp_graph *bp, int size)
{
    int i;

    i = 0;
    while (i < size)
    {
        printf("%d\t%c", bp[i].num, i == size - 1 ? '\n' : ' ');
        i++;
    }
    i = 0;
    while (i < size)
    {
        printf("%d\t%c", bp[i].grey, i == size - 1 ? '\n' : ' ');
        i++;
    }
    i = 0;
    while (i < size)
    {
        printf("%d\t%c", bp[i].black, i == size - 1 ? '\n' : ' ');
        i++;
    }
    i = 0;
    while (i < size)
    {
        printf("%d\t%c", bp[i].cycle_id, i == size - 1 ? '\n' : ' ');
        i++;
    }
}

static void set_trivials(t_bp_graph *bp, int size, int *arr, int n)
{
    int i;

    bp[0].num = 0;
    bp[0].grey = 1;
    bp[0].black = arr[0] * 2 - 1;

    bp[size - 1].num = size - 1;
    bp[size - 1].grey = size - 2;
    bp[size - 1].black = arr[n - 1] * 2;

    i = 0;
    while (i < size)
    {
        bp[i].cycle_id = -1;
        i++;
    }
}

static void set_num_greys(t_bp_graph *bp, int size, int *arr)
{
    int i;
    int arr_num;
    int res;

    i = 1;
    while (i < size - 1)
    {
        arr_num = arr[(i - 1) / 2];
        res = i % 2;
        if (res == 1)
        {
            bp[i].num = arr_num * 2 - 1;
            bp[i].grey = arr_num * 2 - 1 - 1;
        }
        else
        {
            bp[i].num = arr_num * 2;
            bp[i].grey = arr_num * 2 + 1;
        }
        i++;
    }
}

static void set_blacks(t_bp_graph *bp, int size)
{
    int i;
    int res;

    i = 0;
    while (i < size)
    {
        res = i % 2;
        if (res == 0)
        {
            bp[i].black = bp[i + 1].num;
        }
        else
        {
            bp[i].black = bp[i - 1].num;
        }
        i++;
    }
}

static int find_index(t_bp_graph *bp, int size, int target)
{
    int i;

    i = 0;
    while (i < size)
    {
        if (bp[i].num == target)
            return (i);
        i++;
    }
    return (-1);
}

static void seek_cycles(t_bp_graph *bp, int size, int i, int id)
{
    int next;
    int cur;

    cur = i;
    while (bp[cur].cycle_id == -1)
    {
        bp[cur].cycle_id = id;
        next = bp[cur].black;
        cur = find_index(bp, size, next);
        bp[cur].cycle_id = id;
        next = bp[cur].grey;
        cur = find_index(bp, size, next);
    }
}

static void set_cycle_id(t_bp_graph *bp, int size)
{
    int i;
    int id;

    i = 0;
    id = 0;
    while (i < size)
    {
        if (bp[i].cycle_id == -1)
        {
            seek_cycles(bp, size, i, id);
            id++;
        }
        i++;
    }
}

void    bp_graph_init(t_bp_graph *bp, int size, int *arr, int n)
{
    set_trivials(bp, size, arr, n);
    set_num_greys(bp, size, arr);
    set_blacks(bp, size);
    set_cycle_id(bp, size);
}
