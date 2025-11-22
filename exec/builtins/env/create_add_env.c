/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_add_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 19:26:28 by emle-vou          #+#    #+#             */
/*   Updated: 2025/11/22 19:18:42 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	error_env(t_minishell *mini, int i)
{
	while (i >= 0)
	{
		free(mini->env[i]);
		mini->env[i] = NULL;
		i--;
	}
	free(mini->env);
	mini->env = NULL;
}

void	ft_creat_env(t_minishell *mini, char **envp)
{
	int	i;

	if (!envp || !envp[0])
		return ;
	mini->env = ft_malloc(sizeof(char *) * (ft_strlen_y(envp) + 1),
			&mini->alloc);
	if (!mini->env)
		return ;
	i = 0;
	while (envp[i])
	{
		mini->env[i] = ft_malloc(ft_strlen(envp[i]) + 1, &mini->alloc);
		if (!mini->env[i])
		{
			error_env(mini, i - 1);
			return ;
		}
		ft_strlcpy(mini->env[i], envp[i], ft_strlen(envp[i]) + 1);
		i++;
	}
	mini->env[i] = NULL;
}

void	ft_add_env(t_minishell *mini, char *new)
{
	int		i;
	char	**new_mini;
	char	*dst;

	if (!mini->env)
		return ;
	new_mini = ft_malloc(sizeof(char *) * (ft_strlen_y(mini->env) + 2),
			&mini->alloc);
	if (!new_mini)
		return ;
	i = -1;
	while (mini->env[++i])
		new_mini[i] = mini->env[i];
	dst = ft_malloc(ft_strlen(new) + 1, &mini->alloc);
	if (!dst)
	{
		free(new_mini);
		return ;
	}
	ft_strlcpy(dst, new, ft_strlen(new) + 1);
	new_mini[i] = dst;
	new_mini[i + 1] = NULL;
	free(mini->env);
	mini->env = new_mini;
	return ;
}
