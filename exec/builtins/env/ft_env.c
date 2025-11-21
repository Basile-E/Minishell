/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emle-vou <emle-vou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 19:26:40 by emle-vou          #+#    #+#             */
/*   Updated: 2025/11/21 19:38:26 by emle-vou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_minishell *mini)
{
	int	i;

	i = 0;
	if (!mini->env)
		return (0);
	while (mini->env[i])
	{
		ft_putstr_fd(mini->env[i], 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
	return (0);
}
