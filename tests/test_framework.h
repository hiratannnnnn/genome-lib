#ifndef TEST_FRAMEWORK_H
# define TEST_FRAMEWORK_H

# include <stdio.h>

extern int g_passed;
extern int g_failed;

# define ASSERT(cond, msg) do { \
    if (cond) { \
        g_passed++; \
        printf("  [PASS] %s\n", msg); \
    } else { \
        g_failed++; \
        printf("  [FAIL] %s  (%s:%d)\n", msg, __FILE__, __LINE__); \
    } \
} while (0)

# define TEST_GROUP(name) printf("\n=== %s ===\n", name)

#endif
