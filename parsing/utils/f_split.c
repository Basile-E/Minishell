#include "minishell.h"

static size_t	count_words(char const *str, char c);
static char		**alloc_word(char const *s, char c, char **split, size_t s_len);

char	**split_field(char const *s, char c)
{
	size_t	words;
	size_t	s_len;
	char	**split;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (!s)
		return (NULL);
	words = count_words(s, c);
	split = (char **)ft_calloc(sizeof(char *), words + 1);
	if (!split)
		return (NULL);
	split = alloc_word(s, c, split, s_len);
	split[words] = NULL;
	return (split);
}

static size_t	count_words(char const *str, char c)
{
	size_t	words;
	size_t	new_word;
	t_quote	status;

	status = NONE;
	words = 0;
	new_word = 0;
	while (*str)
	{
		set_quote_status(*str, &status);
		if (*str != c && !new_word && status == NONE)
		{
			new_word = 1;
			words++;
		}
		else if (*str == c && status == NONE)
			new_word = 0;
		str++;
	}
	return (words);
}

static void	set_int(size_t *i, size_t *split_i, size_t *word_len,
		t_quote *status)
{
	*i = 0;
	*split_i = 0;
	*word_len = 0;
	*status = NONE;
}

static char	**alloc_word(char const *s, char c, char **split, size_t s_len)
{
	size_t	i;
	size_t	split_i;
	size_t	word_len;
	t_quote	status;

	set_int(&i, &split_i, &word_len, &status);
	while (i < s_len + 1 && s_len > 0)
	{
		set_quote_status(s[i], &status);
		if ((status == NONE && s[i] == c) || !s[i])
		{
			if (word_len > 0)
			{
				split[split_i] = (char *)ft_calloc(word_len + 1, sizeof(char));
				if (split[split_i])
					ft_strlcpy(split[split_i], &s[i - word_len], word_len + 1);
				word_len = 0;
				split_i++;
			}
		}
		else
			word_len++;
		i++;
	}
	return (split);
}
