#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lib.h"
#include "test_framework.h"

/* ---- global state required by lib.h ---- */
size_t  mem      = 0;
size_t  mem_peak = 0;
clock_t proc_start = 0;
clock_t proc_end   = 0;

/* ---- global test counters ---- */
int g_passed = 0;
int g_failed = 0;

/* ---- forward declarations ---- */
void run_tests_matrix(void);
void run_tests_utils_primitives(void);
void run_tests_strings(void);
void run_tests_data_structures(void);
void run_tests_graph_analysis(void);
void run_tests_graph_conversion(void);
void run_tests_graph_gen(void);
void run_tests_sort(void);
void run_tests_memory(void);
void run_tests_io(void);
void run_tests_permutation(void);

int main(void)
{
    srand(42); /* deterministic seed for reproducibility */
    proc_start = clock();

    printf("============================================================\n");
    printf("  graph-lib test suite\n");
    printf("============================================================\n");

    run_tests_memory();           /* first: ensures xmalloc works */
    run_tests_matrix();
    run_tests_utils_primitives();
    run_tests_strings();
    run_tests_data_structures();
    run_tests_graph_analysis();
    run_tests_graph_conversion();
    run_tests_graph_gen();
    run_tests_sort();
    run_tests_io();
    run_tests_permutation();

    proc_end = clock();
    double elapsed = (double)(proc_end - proc_start) / CLOCKS_PER_SEC;

    printf("\n============================================================\n");
    printf("  Results:  %d passed  |  %d failed  |  %.3f s\n",
           g_passed, g_failed, elapsed);
    printf("============================================================\n");

    return (g_failed > 0) ? 1 : 0;
}
