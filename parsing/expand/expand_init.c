/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emle-vou <emle-vou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 22:34:11 by emle-vou          #+#    #+#             */
/*   Updated: 2025/11/23 22:34:32 by emle-vou         ###   ########.fr       */
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
			env_value = ft_strdup(minishell->env[i] + y);
			free(token);
			return (env_value);
		}
		i++;
	}
	return (token);
}

char	*get_env_value(t_minishell *minishell, char *var_name)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(var_name);
	if (ft_strncmp(var_name, "?", 1) == 0 && ft_strlen(var_name) == 1)
		return (ft_itoa(minishell->status));
	if (ft_strncmp(var_name, "$", 1) == 0 && ft_strlen(var_name) == 1)
		return (ft_itoa(getpid()));
	while (minishell->env[i])
	{
		if (ft_strncmp(var_name, minishell->env[i], len) == 0
			&& minishell->env[i][len] == '=')
		{
			return (ft_strdup(minishell->env[i] + len + 1));
		}
		i++;
	}
	return (ft_strdup(""));
}
