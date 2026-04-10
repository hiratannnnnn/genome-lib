/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:10:27 by thirata           #+#    #+#             */
/*   Updated: 2026/04/11 00:11:03 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"
#include "approx_sbpbi.h"

/* Print perm as [a, b, c, ...] */
// static void print_perm(int *perm, int n)
// {
//     int i;

//     printf("[");
//     for (i = 0; i < n; i++)
//         printf("%d%s", perm[i], (i < n - 1) ? ", " : "");
//     printf("]");
// }

// /* ------------------------------------------------------------------ */
// /* Single example: show every step                                     */
// /* ------------------------------------------------------------------ */
// static void demo_step_by_step(void)
// {
//     int perm[] = {3, 1, 4, 5, 2};   /* 0-indexed, n=5 */
//     int n      = 5;
//     int ops[5][3];
//     int copy[5];
//     int num_ops, i;

//     printf("=== step-by-step demo ===\n");
//     printf("input  : ");
//     print_perm(perm, n);
//     printf("  (breakpoints: %d)\n", count_breakpoints(perm, n));

//     copy_permutation(perm, copy, n);
//     num_ops = approx_sbpbi(copy, n, ops);

//     printf("result : ");
//     print_perm(copy, n);
//     printf("  sorted=%s\n", is_identity_permutation(copy, n) ? "yes" : "NO");

//     printf("ops (%d):\n", num_ops);
//     for (i = 0; i < num_ops; i++)
//         printf("  [%d] prefix_block_interchange(arr, n, %d, %d, %d)\n",
//                i + 1, ops[i][0], ops[i][1], ops[i][2]);

//     /* Replay on original to verify */
//     copy_permutation(perm, copy, n);
//     for (i = 0; i < num_ops; i++)
//         prefix_block_interchange(copy, n, ops[i][0], ops[i][1], ops[i][2]);
//     printf("replay : sorted=%s\n\n",
//            is_identity_permutation(copy, n) ? "yes" : "NO");
// }

// /* ------------------------------------------------------------------ */
// /* Stress test: random permutations, check sorting + 2-approx bound   */
// /* ------------------------------------------------------------------ */
// static void demo_stress(int trials, int n)
// {
//     int   *perm;
//     int   *copy;
//     int  (*ops)[3];
//     int    fail, t, num_ops, bp, i;

//     perm = (int *)xmalloc((size_t)n * sizeof(int));
//     copy = (int *)xmalloc((size_t)n * sizeof(int));
//     ops  = (int (*)[3])xmalloc((size_t)n * sizeof(*ops));

//     fail = 0;
//     for (t = 0; t < trials; t++)
//     {
//         /* Generate a random permutation (values 0..n-1) */
//         identity_permutation(perm, n);
//         for (i = n - 1; i > 0; i--)
//         {
//             int j     = rand() % (i + 1);
//             int tmp   = perm[i];
//             perm[i]   = perm[j];
//             perm[j]   = tmp;
//         }

//         bp = count_breakpoints(perm, n);
//         copy_permutation(perm, copy, n);
//         num_ops = approx_sbpbi(copy, n, ops);

//         if (!is_identity_permutation(copy, n))
//         {
//             printf("FAIL (not sorted): ");
//             print_perm(perm, n);
//             printf("\n");
//             fail++;
//         }
//         /* 2-approx bound: ops <= ceil(bp / 3) * 2  (loose check: ops <= bp) */
//         else if (bp > 0 && num_ops > bp)
//         {
//             printf("WARN (ops=%d > bp=%d): ", num_ops, bp);
//             print_perm(perm, n);
//             printf("\n");
//         }
//     }

//     if (fail == 0)
//         printf("stress test: %d trials, n=%d — all sorted correctly\n",
//                trials, n);
//     else
//         printf("stress test: %d/%d FAILED\n", fail, trials);

//     xfree(perm, (size_t)n * sizeof(int));
//     xfree(copy, (size_t)n * sizeof(int));
//     xfree(ops,  (size_t)n * sizeof(*ops));
// }

/* ------------------------------------------------------------------ */

void solve(int *arr, int n);

int main(void)
{
    srand(42);

    int n = 5;

    int *arr;

    arr = rand_perm(n, 1);
    if (!arr)
        return (1);
    solve(arr, n);

    

    free_array_int(arr, n);
    print_info();
    return (0);
}
