/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:40:21 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 20:53:31 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	expandinette(t_minishell *minishell, t_expandinette *exp, char *str)
{
	exp->var_name = extract_var_name(str, exp->i + 1, &exp->end, &minishell);
	exp->var_value = get_env_value(minishell, exp->var_name);
	exp->temp = ft_strjoin(exp->result, exp->var_value);
	free(exp->result);
	exp->result = exp->temp;
	exp->i = exp->end;
	free(exp->var_name);
	free(exp->var_value);
}

void	set_expand(t_expandinette *exp, t_quote *status, t_minishell *mini)
{
	exp->i = 0;
	*status = NONE;
	exp->result = ft_strdup_gc("", mini);
}

char	*do_expand_simple(t_minishell *minishell, char *str)
{
	t_quote			status;
	t_expandinette	exp;

	set_expand(&exp, &status, minishell);
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
			exp.result = exp.temp;
			exp.i++;
		}
	}
	return (exp.result);
}
