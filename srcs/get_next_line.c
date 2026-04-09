/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 16:21:35 by thirata           #+#    #+#             */
/*   Updated: 2025/10/28 16:29:46 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*extract_line(char *stash)
{
	size_t	i;
	size_t	len_nl;
	char	*line;

	if (!stash || !stash[0])
		return (NULL);
	len_nl = ft_strlen_nl(stash);
	line = (char *)malloc(sizeof(char) * (len_nl + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (stash[i] != '\0' && stash[i] != '\n')
	{
		line[i] = stash[i];
		i++;
	}
	if (stash[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*update_stash(char *stash)
{
	size_t	len;
	size_t	len_nl;
	size_t	i;
	char	*new;

	if (!stash)
		return (NULL);
	len = ft_strlen(stash);
	len_nl = ft_strlen_nl(stash);
	if (len == len_nl)
		return (free(stash), NULL);
	new = malloc(sizeof(char) * (len - len_nl + 1));
	if (!new)
		return (free(stash), NULL);
	i = 0;
	while (stash[len_nl])
		new[i++] = stash[len_nl++];
	new[i] = '\0';
	free(stash);
	return (new);
}

static char	*read_to_stash(int fd, char *stash)
{
	char	*buff;
	int		read_bytes;

	buff = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buff)
		return (NULL);
	read_bytes = 1;
	while (!ft_strchr(stash, '\n') && read_bytes != 0)
	{
		read_bytes = read(fd, buff, BUFFER_SIZE);
		if (read_bytes == -1)
			return (free(buff), NULL);
		buff[read_bytes] = '\0';
		stash = ft_strjoin(stash, buff);
		if (!stash)
			return (free(buff), NULL);
	}
	return (free(buff), stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || 10000000 < BUFFER_SIZE)
		return (NULL);
	stash = read_to_stash(fd, stash);
	if (!stash)
		return (NULL);
	line = extract_line(stash);
	stash = update_stash(stash);
	return (line);
}

// int	main(int argc, char **argv)
// {
// 	int		fd;
// 	int		i;
// 	char	*line;

// 	if (argc < 2)
// 	{
// 		printf("Usage: %s <filename>\n", argv[0]);
// 		return (1);
// 	}
// 	fd = open(argv[1], O_RDONLY);
// 	if (fd < 0)
// 	{
// 		perror("open");
// 		return (1);
// 	}
// 	i = 1;
// 	while (i < 10)
// 	{
// 		line = get_next_line(fd);
// 		if (line)
// 		{
// 			printf("line [%02d]: %s", i, line);
// 			free(line);
// 		}
// 		else
// 		{
// 			printf("line [%02d]: (null)\n", i);
// 		}
// 		i++;
// 	}
// 	close(fd);
// 	return (0);
// }
