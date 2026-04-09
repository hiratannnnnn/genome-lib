#include "test_framework.h"
#include "lib.h"

void run_tests_memory(void)
{
    TEST_GROUP("xmalloc / xcalloc / xfree");
    {
        size_t before = mem;

        /* xmalloc increases mem */
        void *p = xmalloc(64);
        ASSERT(p != NULL, "xmalloc(64): non-NULL");
        ASSERT(mem == before + 64, "xmalloc(64): mem counter increased by 64");

        /* xfree decreases mem */
        xfree(p, 64);
        ASSERT(mem == before, "xfree(64): mem counter restored");

        /* xfree(NULL) is safe */
        size_t before2 = mem;
        xfree(NULL, 32);
        ASSERT(mem == before2, "xfree(NULL): no change to mem counter");

        /* xcalloc zeros out memory */
        int *arr = (int *)xcalloc(5, sizeof(int));
        ASSERT(arr != NULL, "xcalloc(5, int): non-NULL");
        int all_zero = 1;
        for (int i = 0; i < 5; i++)
            if (arr[i] != 0) all_zero = 0;
        ASSERT(all_zero, "xcalloc: memory zeroed");
        xfree(arr, sizeof(int) * 5);
    }

    TEST_GROUP("xrealloc");
    {
        size_t before = mem;
        int *p = (int *)xmalloc(sizeof(int) * 4);
        p[0] = 1; p[1] = 2; p[2] = 3; p[3] = 4;

        /* Grow */
        int *p2 = (int *)xrealloc(p, sizeof(int) * 4, sizeof(int) * 8);
        ASSERT(p2 != NULL, "xrealloc(grow): non-NULL");
        ASSERT(mem == before + sizeof(int) * 8, "xrealloc(grow): mem updated correctly");
        ASSERT(p2[0] == 1 && p2[3] == 4, "xrealloc(grow): original data preserved");

        /* Shrink */
        int *p3 = (int *)xrealloc(p2, sizeof(int) * 8, sizeof(int) * 2);
        ASSERT(p3 != NULL, "xrealloc(shrink): non-NULL");
        ASSERT(mem == before + sizeof(int) * 2, "xrealloc(shrink): mem updated correctly");
        ASSERT(p3[0] == 1 && p3[1] == 2, "xrealloc(shrink): data preserved within new size");

        /* new_size = 0 -> frees and returns NULL */
        void *p4 = xrealloc(p3, sizeof(int) * 2, 0);
        ASSERT(p4 == NULL, "xrealloc(0): returns NULL");
        ASSERT(mem == before, "xrealloc(0): mem fully restored");
    }

    TEST_GROUP("mem_peak tracking");
    {
        size_t peak_before = mem_peak;
        size_t mem_before  = mem;

        /* Allocate a large block to drive up peak */
        void *big = xmalloc(1024);
        ASSERT(mem_peak >= mem, "mem_peak: always >= current mem");

        xfree(big, 1024);
        ASSERT(mem == mem_before, "mem_peak test: mem restored after free");
        /* peak should not decrease */
        ASSERT(mem_peak >= peak_before, "mem_peak: never decreases");
    }
}
