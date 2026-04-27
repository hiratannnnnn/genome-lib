#ifndef TEST_FRAMEWORK_H
# define TEST_FRAMEWORK_H

# include <stdio.h>

extern int	g_passed;
extern int	g_failed;

# define CLR_RESET "\033[0m"
# define CLR_PASS "\033[32m"
# define CLR_FAIL "\033[31m"
# define CLR_GROUP "\033[36m"
# define CLR_BOLD "\033[1m"
# define CLR_DIM "\033[2m"

static inline void	t_pass(const char *msg)
{
	g_passed++;
	printf("  " CLR_PASS "[PASS]" CLR_RESET " %s\n", msg);
}

static inline void	t_fail(const char *msg, const char *file, int line)
{
	g_failed++;
	printf("  " CLR_FAIL CLR_BOLD "[FAIL]" CLR_RESET " %s", msg);
	printf(CLR_DIM "  (%s:%d)" CLR_RESET "\n", file, line);
}

static inline void	t_check(int ok, const char *msg, const char *file, int line)
{
	if (ok)
		t_pass(msg);
	else
		t_fail(msg, file, line);
}

static inline void	t_group(const char *name)
{
	printf("\n" CLR_GROUP "=== %s ===" CLR_RESET "\n", name);
}

# define ASSERT(c, m) t_check((c), (m), __FILE__, __LINE__)
# define TEST_GROUP(n) t_group(n)

#endif
