/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emle-vou <emle-vou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 19:28:00 by emle-vou          #+#    #+#             */
/*   Updated: 2025/11/23 19:20:47 by emle-vou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	error_exp(char **exp, int i)
{
	while (i >= 0)
	{
		free(exp[i]);
		exp[i] = NULL;
		i--;
	}
	free(exp);
}

char	**ft_create_export(t_minishell *mini)
{
	int		i;
	char	**exp;

	if (!mini->env)
		return (0);
	exp = ft_malloc((sizeof(char *) * (ft_strlen_y(mini->env) + 1)),
			&mini->alloc);
	i = 0;
	while (mini->env[i])
	{
		exp[i] = ft_malloc((ft_strlen(mini->env[i]) + 1), &mini->alloc);
		if (!exp[i])
		{
			error_exp(exp, i - 1);
			return (NULL);
		}
		ft_strlcpy(exp[i], mini->env[i], ft_strlen(mini->env[i]) + 1);
		i++;
	}
	exp[i] = NULL;
	return (exp);
}

void	ft_sort_export(char **exp)
{
	int		i;
	int		j;
	char	*swap;

	j = 0;
	i = 0;
	if (!exp)
		return ;
	while (exp[i])
	{
		j = 1;
		while (exp[j])
		{
			if (ft_strcmp_equal(exp[j - 1], exp[j]) > 0)
			{
				swap = exp[j - 1];
				exp[j - 1] = exp[j];
				exp[j] = swap;
			}
			j++;
		}
		i++;
	}
}

void	ft_export(t_minishell *mini, char **cmd)
{
	int		i;
	char	**exp;

	if (!mini || !mini->env)
		return ;
	if (cmd && cmd[1])
	{
		i = 1;
		while (cmd[i])
		{
			ft_update_env(mini, cmd[i]);
			i++;
		}
		return ;
	}
	exp = ft_create_export(mini);
	if (!exp)
		return ;
	ft_sort_export(exp);
	i = 0;
	while (exp[i])
	{
		ft_putstr_fd("export ", 1);
		ft_putstr_fd(exp[i], 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
	return ;
}
