#include "lib.h"
#include "test_framework.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ---- global test counters ---- */
int		g_passed = 0;
int		g_failed = 0;

/* ---- forward declarations ---- */
void	run_tests_matrix(void);
void	run_tests_utils_primitives(void);
void	run_tests_strings(void);
void	run_tests_data_structures(void);
void	run_tests_graph_analysis(void);
void	run_tests_graph_conversion(void);
void	run_tests_graph_gen(void);
void	run_tests_sort(void);
void	run_tests_memory(void);
void	run_tests_io(void);
void	run_tests_permutation(void);

int	main(void)
{
	double	elapsed;
	clock_t	proc_start;
	clock_t	proc_end;

	srand(42);
	proc_start = clock();
	printf(CLR_BOLD
		"============================================================\n");
	printf("  graph-lib test suite\n");
	printf("============================================================"
		CLR_RESET "\n");
	run_tests_memory();
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
	elapsed = (double)(proc_end - proc_start) / CLOCKS_PER_SEC;
	printf("\n" CLR_BOLD
		"============================================================\n");
	if (g_failed == 0)
		printf("  Results:  " CLR_PASS "%d passed" CLR_RESET CLR_BOLD
			"  |  0 failed  |  " CLR_DIM "%.3f s\n" CLR_RESET, g_passed,
			elapsed);
	else
		printf("  Results:  " CLR_PASS "%d passed" CLR_RESET CLR_BOLD
			"  |  " CLR_FAIL "%d failed" CLR_RESET CLR_BOLD "  |  "
			CLR_DIM "%.3f s\n" CLR_RESET, g_passed, g_failed, elapsed);
	printf(CLR_BOLD "============================================================"
		CLR_RESET "\n");
	return ((g_failed > 0) ? 1 : 0);
}
