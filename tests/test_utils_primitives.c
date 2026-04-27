#include "lib.h"
#include "test_framework.h"

/* ---- helpers ---- */

static int	is_valid_perm(int *arr, int n)
{
	int	*seen;

	seen = (int *)calloc(n, sizeof(int));
	if (!seen)
		return (0);
	for (int i = 0; i < n; i++)
	{
		if (arr[i] < 0 || arr[i] >= n || seen[arr[i]])
		{
			free(seen);
			return (0);
		}
		seen[arr[i]] = 1;
	}
	free(seen);
	return (1);
}

/* ---- tests ---- */

void	run_tests_utils_primitives(void)
{
	int		arr5[] = {1, 2, 3, 4, 5};
	int		arr0[] = {0, 0, 0};
	int		arr_neg[] = {-1, 2, -3};
	int		**m;
	int		**mz;
	int		arr[] = {3, 1, 4, 1, 5, 9, 2, 6};
	int		single[] = {42};
	int		neg[] = {-5, -1, -3, -2};
	int		*p5;
	int		*p1;
	int		*p10;
	char	**ss;

	TEST_GROUP("sum_array / sum_matrix");
	{
		ASSERT(sum_array(arr5, 5) == 15, "sum_array: {1,2,3,4,5} = 15");
		ASSERT(sum_array(arr0, 3) == 0, "sum_array: all zeros = 0");
		ASSERT(sum_array(arr_neg, 3) == -2, "sum_array: with negatives");
		m = gen_matrix_int(2, 3);
		m[0][0] = 1;
		m[0][1] = 2;
		m[0][2] = 3;
		m[1][0] = 4;
		m[1][1] = 5;
		m[1][2] = 6;
		ASSERT(sum_matrix(m, 2, 3) == 21, "sum_matrix: 2x3 sum = 21");
		free_matrix_int(m, 2, 3);
		mz = gen_matrix_int(3, 3);
		ASSERT(sum_matrix(mz, 3, 3) == 0, "sum_matrix: zero matrix = 0");
		free_matrix_int(mz, 3, 3);
	}
	TEST_GROUP("min_int / max_int");
	{
		ASSERT(min_int(3, 7) == 3, "min_int(3,7) = 3");
		ASSERT(min_int(7, 3) == 3, "min_int(7,3) = 3");
		ASSERT(min_int(5, 5) == 5, "min_int(5,5) = 5");
		ASSERT(min_int(-1, 0) == -1, "min_int(-1,0) = -1");
		ASSERT(max_int(3, 7) == 7, "max_int(3,7) = 7");
		ASSERT(max_int(7, 3) == 7, "max_int(7,3) = 7");
		ASSERT(max_int(5, 5) == 5, "max_int(5,5) = 5");
		ASSERT(max_int(-1, 0) == 0, "max_int(-1,0) = 0");
	}
	TEST_GROUP("min_double / max_double");
	{
		ASSERT(min_double(1.0, 2.0) == 1.0, "min_double(1.0,2.0) = 1.0");
		ASSERT(min_double(2.0, 1.0) == 1.0, "min_double(2.0,1.0) = 1.0");
		ASSERT(min_double(3.0, 3.0) == 3.0, "min_double(3.0,3.0) = 3.0");
		ASSERT(max_double(1.0, 2.0) == 2.0, "max_double(1.0,2.0) = 2.0");
		ASSERT(max_double(2.0, 1.0) == 2.0, "max_double(2.0,1.0) = 2.0");
		ASSERT(max_double(3.0, 3.0) == 3.0, "max_double(3.0,3.0) = 3.0");
	}
	TEST_GROUP("max_of_array / min_of_array / min_of_array_index");
	{
		ASSERT(max_of_array(arr, 8) == 9, "max_of_array: max is 9");
		ASSERT(min_of_array(arr, 8) == 1, "min_of_array: min is 1");
		ASSERT(min_of_array_index(arr, 8) == 1,
			"min_of_array_index: first min at index 1");
		ASSERT(max_of_array(single, 1) == 42, "max_of_array(single): 42");
		ASSERT(min_of_array(single, 1) == 42, "min_of_array(single): 42");
		ASSERT(min_of_array_index(single, 1) == 0,
			"min_of_array_index(single): index 0");
		ASSERT(min_of_array(neg, 4) == -5, "min_of_array: negatives - min is
			-5");
		ASSERT(max_of_array(neg, 4) == -1, "max_of_array: negatives - max is
			-1");
	}
	TEST_GROUP("rand_perm");
	{
		p5 = rand_perm(5, 0);
		ASSERT(p5 != NULL, "rand_perm(5, 0): returns non-NULL");
		ASSERT(is_valid_perm(p5, 5),
			"rand_perm(5,0): valid permutation of [0..4]");
		free(p5);
		p1 = rand_perm(1, 0);
		ASSERT(p1 != NULL, "rand_perm(1, 0): returns non-NULL");
		ASSERT(p1[0] == 0, "rand_perm(1, 0): single element is 0");
		free(p1);
		/* Multiple calls should (eventually) differ — not strictly testable,
			but we at least verify structural validity each time */
		p10 = rand_perm(10, 0);
		ASSERT(p10 != NULL, "rand_perm(10, 0): returns non-NULL");
		ASSERT(is_valid_perm(p10, 10),
			"rand_perm(10,0): valid permutation of [0..9]");
		free(p10);
	}
	TEST_GROUP("count_digit");
	{
		ASSERT(count_digit(0) == 1, "count_digit(0) = 1");
		ASSERT(count_digit(9) == 1, "count_digit(9) = 1");
		ASSERT(count_digit(10) == 2, "count_digit(10) = 2");
		ASSERT(count_digit(100) == 3, "count_digit(100) = 3");
		ASSERT(count_digit(999) == 3, "count_digit(999) = 3");
		ASSERT(count_digit(1000) == 4, "count_digit(1000) = 4");
		ASSERT(count_digit(-1) == 2, "count_digit(-1) = 2 (sign + digit)");
		ASSERT(count_digit(-99) == 3, "count_digit(-99) = 3");
		ASSERT(count_digit(-2147483648) == 11, "count_digit(INT_MIN) = 11");
	}
	TEST_GROUP("free_array_char");
	{
		/* Smoke test: allocate char** and free without crash */
		ss = (char **)xmalloc(sizeof(char *) * 3);
		ss[0] = (char *)xmalloc(sizeof(char) * 4);
		ss[1] = (char *)xmalloc(sizeof(char) * 4);
		ss[2] = (char *)xmalloc(sizeof(char) * 4);
		strcpy(ss[0], "abc");
		strcpy(ss[1], "def");
		strcpy(ss[2], "ghi");
		free_array_char(ss, 3); /* must not crash */
		ASSERT(1, "free_array_char: no crash on valid input");
	}
}
