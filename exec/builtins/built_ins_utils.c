/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emle-vou <emle-vou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 19:27:12 by emle-vou          #+#    #+#             */
/*   Updated: 2025/11/21 19:27:14 by emle-vou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_error(const char *str, const char *error)
{
	if (error)
		ft_printf("minishell: %s: %s\n", str, error);
	else
		ft_printf("minishell: error: %s\n", str);
}

int	ft_strcmp_equal(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i] && s1[i] != '=' && s2[i] != '=')
		i++;
	if ((s1[i] == '=' || s1[i] == 0) && (s2[i] == '=' || s2[i] == 0))
		return (0);
	if (s2[i] == '=')
		return (1);
	if (s1[i] == '=')
		return (-1);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	does_it_exist(t_minishell *mini, char *src)
{
	int	i;

	i = 0;
	if (!mini->env)
		return (1);
	while (mini->env[i])
	{
		if (ft_strcmp_equal(src, mini->env[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int	ft_strlen_y(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}
