/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 08:10:54 by thirata           #+#    #+#             */
/*   Updated: 2026/04/10 08:10:55 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

static int	is_set(char const c, char const *set)
{
	int	len;
	int	i;

	i = 0;
	len = strlen(set);
	while (i < len)
		if (set[i++] == c)
			return (1);
	return (0);
}

static int	calc_size(int const start, int const end)
{
	int	size;

	if (start > end)
		size = 0;
	else
		size = end - start + 1;
	return (size);
}

static void	put_word(char *res, int const start, int const size, char const *s)
{
	int	i;

	i = 0;
	while (i < size)
	{
		res[i] = s[start + i];
		i++;
	}
	res[i] = '\0';
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;

	int start, end, size;
	if (!s1 || !set)
		return (NULL);
	start = 0;
	end = strlen(s1) - 1;
	if (end == (int)-1)
		return (strdup(""));
	while (is_set(s1[start], set))
		start++;
	while (start < end && is_set(s1[end], set))
		end--;
	size = calc_size(start, end);
	res = (char *)malloc(sizeof(char) * (size + 1));
	if (!res)
		return (NULL);
	put_word(res, start, size, s1);
	return (res);
}
