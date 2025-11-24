/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:39:39 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 20:41:11 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_quote(t_r_quote *r_quote, char *str)
{
	r_quote->i = 0;
	r_quote->j = 0;
	r_quote->len = ft_strlen(str);
	r_quote->status = NONE;
	r_quote->quote_char = '\0';
	r_quote->result = malloc(r_quote->len + 1);
}

char	*remove_quotes(char *str)
{
	t_r_quote	r_quote;

	set_quote(&r_quote, str);
	if (!r_quote.result)
		return (NULL);
	while (str[r_quote.i])
	{
		if (r_quote.status == NONE && (str[r_quote.i] == '\''
				|| str[r_quote.i] == '"'))
		{
			r_quote.quote_char = str[r_quote.i];
			r_quote.status = (str[r_quote.i] == '\'') ? SINGLE : DOUBLE;
		}
		else if (r_quote.status != NONE && str[r_quote.i] == r_quote.quote_char)
		{
			r_quote.status = NONE;
			r_quote.quote_char = '\0';
		}
		else
			r_quote.result[r_quote.j++] = str[r_quote.i];
		r_quote.i++;
	}
	if (str)
		free(str);
	r_quote.result[r_quote.j] = '\0';
	return (r_quote.result);
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
			return (ft_strdup_gc(minishell->env[i] + len + 1, minishell));
		}
		i++;
	}
	return (ft_strdup_gc("", minishell));
}

char	*extract_var_name(char *str, int start, int *end, t_minishell **mini)
{
	int	i;

	i = start;
	if (str[i] == '?' || str[i] == '$')
	{
		*end = i + 1;
		return (ft_substr(str, i, 1));
	}
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*end = i;
	if (i == start)
		return (ft_strdup_gc("", *mini));
	return (ft_substr(str, start, i - start));
}

int	check_dolar_sign(char *str, int i)
{
	if (str[i] == '$' && str[i + 1] && str[i + 1] != '"')
		return (0);
	return (1);
}
