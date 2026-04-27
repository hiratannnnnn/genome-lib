#ifndef TEST_FRAMEWORK_H
# define TEST_FRAMEWORK_H

# include <stdio.h>

extern int g_passed;
extern int g_failed;

# define CLR_RESET  "\033[0m"
# define CLR_PASS   "\033[32m"
# define CLR_FAIL   "\033[31m"
# define CLR_GROUP  "\033[36m"
# define CLR_BOLD   "\033[1m"
# define CLR_DIM    "\033[2m"

# define ASSERT(cond, msg) do { \
    if (cond) { \
        g_passed++; \
        printf("  " CLR_PASS "[PASS]" CLR_RESET " %s\n", msg); \
    } else { \
        g_failed++; \
        printf("  " CLR_FAIL CLR_BOLD "[FAIL]" CLR_RESET " %s" \
            CLR_DIM "  (%s:%d)" CLR_RESET "\n", msg, __FILE__, __LINE__); \
    } \
} while (0)

# define TEST_GROUP(name) printf("\n" CLR_GROUP "=== %s ===" CLR_RESET "\n", name)

#endif
