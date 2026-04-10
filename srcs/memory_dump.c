/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_dump.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:11:34 by thirata           #+#    #+#             */
/*   Updated: 2026/04/10 08:11:35 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

void dump_memory_hex(const void *ptr, int size, int offset)
{
	const unsigned char *data = (const unsigned char *) ptr;
	unsigned char c;
	int i, j;

	if (!ptr)
	{
		printf("NULL pointer\n");
		return ;
	}

	printf("Memory dump at %p (size: %d bytes, offset: %d):\n", ptr, size, offset);
    printf("Offset   00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F  ASCII\n");
    printf("------   -----------------------------------------------  ----------------\n");

	for (i = 0; i < size; i += 16)
	{
		printf("%06X:  ", offset + i);
		for (j = 0; j < 16; j++)
		{
			if (i + j < size)
				printf("%02X ", data[i + j]);
			else
				printf("   ");
			if (j == 7)
				printf(" ");
		}
		printf(" ");

		for (j = 0; j < 16 && i + j < size; j++)
		{
			c = data[i + j];
			printf("%c", (c >= 32 && c <= 126) ?
					c : '.');
		}
		printf("\n");
	}
	printf("\n");
}

void dump_memory_detailed(const void *ptr, int size, const char *label)
{
	const unsigned char *data = (const unsigned char *) ptr;
	int zero_c;
	int ff_c;
	int pattern_c;
	int i;

	if (!ptr)
	{
		printf("Memory dump [%s]: NULL pointer\n",
				label ? label : "Unknown");
		return ;
	}
	printf("=== Memory Dump: %s ===\n",
				label ? label : "Unknown");
    printf("Address: %p\n", ptr);
    printf("Size: %d bytes\n", size);
    printf("Memory pattern analysis:\n");
	zero_c = ff_c = pattern_c = 0;
	for (i = 0; i < size; i++)
	{
		if (data[i] == 0x00)
			zero_c++;
		else if (data[i] == 0xFF)
			ff_c++;
		else if (i > 0 && data[i] == data[i - 1])
			pattern_c++;
	}
	printf("  - Zero bytes: %d (%.1f%%)\n", zero_c,
        (double)zero_c / size * 100);
    printf("  - 0xFF bytes: %d (%.1f%%)\n", ff_c,
        (double)ff_c / size * 100);
    printf("  - Pattern repetition: %d (%.1f%%)\n", pattern_c,
        (double)pattern_c / size * 100);
    dump_memory_hex(ptr, size, 0);
}
