/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thirata <thirata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 16:21:48 by thirata           #+#    #+#             */
/*   Updated: 2025/10/25 22:08:26 by thirata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

// get_next_line_utils.c
size_t	ft_strlen(char const *s);
size_t	ft_strlen_nl(char const *s);
char	*ft_strchr(char *s, char c);
char	*ft_strjoin(char *st, char *bf);

// get_next_line.c
char	*get_next_line(int fd);

#endif
