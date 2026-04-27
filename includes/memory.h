#ifndef MEMORY_H
# define MEMORY_H

# include <stddef.h>
# include <time.h>

/* ========= Global State ========= */

extern size_t	mem;
extern size_t	mem_peak;
extern clock_t	proc_start;
extern clock_t	proc_end;

/* ========= Memory Allocation ========= */

// xmalloc.c
void	*xmalloc	(size_t size);
void	*xcalloc	(int n, size_t size_each);
void	xfree		(void *ptr, size_t size);
void	*xrealloc	(void *ptr, size_t old_size, size_t new_size);

/* ========= Memory Diagnostics ========= */

// memory_dump.c
void	dump_memory_hex		(const void *ptr, int size, int offset);
void	dump_memory_detailed(const void *ptr, int size, const char *label);

#endif
