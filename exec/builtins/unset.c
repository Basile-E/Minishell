
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emle-vou <emle-vou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 19:28:13 by emle-vou          #+#    #+#             */
/*   Updated: 2025/11/22 19:57:31 by emle-vou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_line_env(t_minishell *mini, char *str)
{
	int		i;
	int		j;
	char	**new_mini;

	new_mini = malloc(sizeof(char *) * (ft_strlen_y(mini->env) + 1));
	if (!new_mini)
		return (ft_error("rm line env", strerror(errno)));
	i = 0;
	j = 0;
	while (mini->env[i])
	{
		if (ft_strcmp_equal(str, mini->env[i]) == 0)
			free(mini->env[i]);
		else
		{
			new_mini[j] = mini->env[i];
			j++;
		}
		i++;
	}
	new_mini[j] = NULL;
	if (mini->env)
		free(mini->env);
	mini->env = new_mini;
}

static int	does_it_exist_env(t_minishell *mini, char *src)
{
	int	i;

	i = 0;
	while (mini->env[i])
	{
		if (ft_strcmp_equal(src, mini->env[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int	ft_unset(t_minishell *mini, char **args)
{
	int	i;

	i = 0;
	if (args == NULL)
		return (0);
	while (args[i])
	{
		if (does_it_exist_env(mini, args[i]) == 0)
			remove_line_env(mini, args[i]);
		i++;
	}
	return (0);
}
