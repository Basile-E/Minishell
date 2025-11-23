/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emle-vou <emle-vou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 22:14:10 by emle-vou          #+#    #+#             */
/*   Updated: 2025/11/23 22:34:00 by emle-vou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*extract_var_name(char *str, int start, int *end)
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
		return (ft_strdup(""));
	return (ft_substr(str, start, i - start));
}

int	check_dolar_sign(char *str, int i)
{
	if (str[i] == '$' && str[i + 1] && str[i + 1] != '"')
		return (0);
	return (1);
}

void	expandinette(t_minishell *minishell, t_expandinette *exp, char *str)
{
	exp->var_name = extract_var_name(str, exp->i + 1, &exp->end);
	exp->var_value = get_env_value(minishell, exp->var_name);
	exp->temp = ft_strjoin(exp->result, exp->var_value);
	free(exp->result);
	exp->result = exp->temp;
	exp->i = exp->end;
	free(exp->var_name);
	free(exp->var_value);
}

void	set_expand(t_expandinette *exp, t_quote *status)
{
	exp->i = 0;
	*status = NONE;
	exp->result = ft_strdup("");
}

char	*do_expand_simple(t_minishell *minishell, char *str)
{
	t_quote			status;
	t_expandinette	exp;

	set_expand(&exp, &status);
	if (!exp.result)
		return (NULL);
	while (str[exp.i])
	{
		set_quote_status(str[exp.i], &status);
		if (str[exp.i] == '$' && str[exp.i + 1] && str[exp.i + 1] != '"'
			&& str[exp.i + 1] != '\'' && status != SINGLE)
			expandinette(minishell, &exp, str);
		else
		{
			exp.char_str[0] = str[exp.i];
			exp.char_str[1] = '\0';
			exp.temp = ft_strjoin(exp.result, exp.char_str);
			free(exp.result);
			exp.result = exp.temp;
			exp.i++;
		}
	}
	return (exp.result);
}
