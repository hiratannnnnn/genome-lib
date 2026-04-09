#include "lib.h"

static int is_sep(char c, char *sep)
{
	while (*sep)
	{
		if (c == *sep)
			return (1);
		sep++;
	}
	return (0);
}

static int count_words(char *str, char *sep)
{
	int count;
	int in_word;
	count = 0;
	in_word = 0;

	while (*str)
	{
		if (is_sep(*str, sep))
			in_word = 0;
		else if(!in_word)
		{
			in_word = 1;
			count++;
		}
		str++;
	}
	return (count);
}

static char *copy_word(char *start, char const *end)
{
	char *word;
	int len, i;

	len = end - start;
	word = (char *)xmalloc(sizeof(char) * (len + 1));
	i = 0;
	if (!word)
		return (NULL);
	while (i < len)
	{
		word[i] = start[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char **ft_split(char const *str, char *sep)
{
	char **dd;
	int word_c, i;
	char *start;

	word_c = count_words((char *)str, sep);
	i = 0;
	dd = (char **)xmalloc(sizeof(char *) * (word_c + 1));
	if (!dd)
		return NULL;
	while (*str)
	{
		while (*str && is_sep(*str, sep))
			str++;
		start = (char *)str;
		while (*str && !is_sep(*str, sep))
			str++;
		if (start != str)
			dd[i++] = copy_word(start, str);
	}
	dd[i] = NULL;
	return (dd);
}
