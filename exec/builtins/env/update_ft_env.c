/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_ft_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: basile <basile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 19:27:02 by emle-vou          #+#    #+#             */
/*   Updated: 2025/11/22 06:24:24 by basile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_update_env(t_minishell *mini, char *new)
{
	int		i;
	char	*dst;

	if (!mini->env)
		return (ft_add_env(mini, new));
	i = 0;
	while (mini->env[i])
	{
		if (ft_strcmp_equal(new, mini->env[i]) == 0)
			break ;
		i++;
	}
	if (!mini->env[i])
		return (ft_add_env(mini, new));
	free(mini->env[i]);
	dst = ft_malloc(ft_strlen(new) + 1, &mini->alloc);
	if (!dst)
		return (ft_error("update env", strerror(errno)), _exit(1));
	ft_strlcpy(dst, new, ft_strlen(new) + 1);
	mini->env[i] = dst;
}
