/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 16:21:53 by thirata           #+#    #+#             */
/*   Updated: 2025/10/25 21:41:46 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(char const *s)
{
	size_t	len;

	len = 0;
	if (!s)
		return (0);
	while (s[len])
		len++;
	return (len);
}

size_t	ft_strlen_nl(char const *s)
{
	size_t	len;

	len = 0;
	if (!s)
		return (0);
	while (s[len] != '\0' && s[len] != '\n')
		len++;
	if (s[len] == '\n')
		len++;
	return (len);
}

char	*ft_strchr(char *s, char c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == c)
			return (s);
		s++;
	}
	if (c == '\0')
		return (s);
	return (NULL);
}

char	*ft_strjoin(char *st, char *bf)
{
	char	*new;
	size_t	i;
	size_t	j;

	if (!bf)
		return (NULL);
	if (!st)
	{
		st = (char *)malloc(sizeof(char) * 1);
		if (!st)
			return (NULL);
		st[0] = '\0';
	}
	new = (char *)malloc(sizeof(char) * (ft_strlen(st) + ft_strlen(bf) + 1));
	if (!new)
		return (NULL);
	i = -1;
	j = 0;
	while (st[++i])
		new[i] = st[i];
	while (bf[j])
		new[i++] = bf[j++];
	new[i] = '\0';
	free(st);
	return (new);
}
