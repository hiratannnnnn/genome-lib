#include "lib.h"
#include "test_framework.h"

void	run_tests_memory(void)
{
	size_t	before;
	void	*p;
	size_t	before2;
	int		*arr;
	int		all_zero;
	int		*ip;
	int		*ip2;
	int		*ip3;
	void	*p4;
	size_t	peak_before;
	size_t	mem_before;
	void	*big;

	TEST_GROUP("xmalloc / xcalloc / xfree");
	{
		before = mem;
		/* xmalloc increases mem */
		p = xmalloc(64);
		ASSERT(p != NULL, "xmalloc(64): non-NULL");
		ASSERT(mem == before + 64, "xmalloc(64): mem counter increased by 64");
		/* xfree decreases mem */
		xfree(p, 64);
		ASSERT(mem == before, "xfree(64): mem counter restored");
		/* xfree(NULL) is safe */
		before2 = mem;
		xfree(NULL, 32);
		ASSERT(mem == before2, "xfree(NULL): no change to mem counter");
		/* xcalloc zeros out memory */
		arr = (int *)xcalloc(5, sizeof(int));
		ASSERT(arr != NULL, "xcalloc(5, int): non-NULL");
		all_zero = 1;
		for (int i = 0; i < 5; i++)
			if (arr[i] != 0)
				all_zero = 0;
		ASSERT(all_zero, "xcalloc: memory zeroed");
		xfree(arr, sizeof(int) * 5);
	}
	TEST_GROUP("xrealloc");
	{
		before = mem;
		ip = (int *)xmalloc(sizeof(int) * 4);
		ip[0] = 1;
		ip[1] = 2;
		ip[2] = 3;
		ip[3] = 4;
		/* Grow */
		ip2 = (int *)xrealloc(ip, sizeof(int) * 4, sizeof(int) * 8);
		ASSERT(ip2 != NULL, "xrealloc(grow): non-NULL");
		ASSERT(mem == before + sizeof(int) * 8,
			"xrealloc(grow): mem updated correctly");
		ASSERT(ip2[0] == 1 && ip2[3] == 4,
			"xrealloc(grow): original data preserved");
		/* Shrink */
		ip3 = (int *)xrealloc(ip2, sizeof(int) * 8, sizeof(int) * 2);
		ASSERT(ip3 != NULL, "xrealloc(shrink): non-NULL");
		ASSERT(mem == before + sizeof(int) * 2,
			"xrealloc(shrink): mem updated correctly");
		ASSERT(ip3[0] == 1 && ip3[1] == 2,
			"xrealloc(shrink): data preserved within new size");
		/* new_size = 0 -> frees and returns NULL */
		p4 = xrealloc(ip3, sizeof(int) * 2, 0);
		ASSERT(p4 == NULL, "xrealloc(0): returns NULL");
		ASSERT(mem == before, "xrealloc(0): mem fully restored");
	}
	TEST_GROUP("mem_peak tracking");
	{
		peak_before = mem_peak;
		mem_before = mem;
		/* Allocate a large block to drive up peak */
		big = xmalloc(1024);
		ASSERT(mem_peak >= mem, "mem_peak: always >= current mem");
		xfree(big, 1024);
		ASSERT(mem == mem_before, "mem_peak test: mem restored after free");
		/* peak should not decrease */
		ASSERT(mem_peak >= peak_before, "mem_peak: never decreases");
	}
}
