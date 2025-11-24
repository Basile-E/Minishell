/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expandinette.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:36:21 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 20:37:40 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_for_expand(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (i + 1);
		i++;
	}
	return (0);
}

int	get_word_len(char *str)
{
	int	len;

	len = 0;
	while (str[len] && is_char(str[len]))
		len++;
	return (len);
}

char	*get_expand_token(char *str, int beg_token)
{
	int		j;
	char	*res;
	int		len;

	beg_token++;
	len = get_word_len(str + beg_token);
	res = malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	j = 0;
	while (j < len && is_char(str[beg_token]))
	{
		res[j] = str[beg_token];
		beg_token++;
		j++;
	}
	res[j] = '\0';
	return (res);
}

char	*do_expand(t_minishell *minishell, char *token)
{
	char	*env_value;
	int		i;
	int		y;

	i = 0;
	while (minishell->env[i])
	{
		if (ft_strncmp(token, minishell->env[i], ft_strlen(token)) == 0
			&& minishell->env[i][ft_strlen(token)] == '=')
		{
			y = ft_strlen(token) + 1;
			env_value = ft_strdup_gc(minishell->env[i] + y, minishell);
			free(token);
			return (env_value);
		}
		i++;
	}
	return (token);
}

void	set_quote_status(char c, t_quote *status)
{
	if (*status == NONE && c == '\'')
		*status = SINGLE;
	else if (*status == NONE && c == '"')
		*status = DOUBLE;
	else if ((*status == SINGLE && c == '\'') || (*status == DOUBLE
		&& c == '"'))
		*status = NONE;
}
