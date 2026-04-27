/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:12:45 by thirata           #+#    #+#             */
/*   Updated: 2026/04/10 08:12:46 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

void	*xmalloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		fprintf(stderr, "failed to allocate memory size of %d.\n", (int)size);
		return (NULL);
	}
	mem += size;
	if (mem > mem_peak)
		mem_peak = mem;
	return (ptr);
}

void	*xcalloc(int n, size_t size_each)
{
	void	*ptr;

	ptr = xmalloc(size_each * n);
	if (!ptr)
	{
		fprintf(stderr, "failed to allocate memory size of %d.\n",
			(int)size_each * n);
		return (NULL);
	}
	memset(ptr, 0, n * size_each);
	return (ptr);
}

void	xfree(void *ptr, size_t size)
{
	if (ptr)
	{
		free(ptr);
		mem -= size;
	}
}

void	*xrealloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (new_size == 0)
		return (xfree(ptr, old_size), NULL);
	new_ptr = realloc(ptr, new_size);
	if (!new_ptr)
		return (fprintf(stderr, "failed to reallocate memory size of %d.\n",
				(int)new_size), NULL);
	mem += (new_size - old_size);
	if (mem > mem_peak)
		mem_peak = mem;
	return (new_ptr);
}
