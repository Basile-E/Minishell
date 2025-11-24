/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 19:27:42 by emle-vou          #+#    #+#             */
/*   Updated: 2025/11/24 18:08:35 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_n_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	i = 1;
	if (str[i] != 'n')
		return (0);
	while (str[i] == 'n')
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

int	ft_echo(t_minishell mini, t_cmd *cmd)
{
	int	i;
	int	n_flag;
	int temp;

	if(cmd->fd_out == -1)
		temp = cmd->fd_out + 2;
	else
		temp = cmd->fd_out;
	(void)mini;
	if (!cmd->args)
		return (0);
	i = 1;
	n_flag = 1;
	while (cmd->args[i] && ft_n_flag(cmd->args[i]))
	{
		n_flag = 0;
		i++;
	}
	printf("line = %s", cmd->args[i]);
	while (cmd->args[i])
	{
		printf(RED "fd out = %d" RESET, temp);
		ft_putstr_fd(cmd->args[i], temp);
		if (cmd->args[i + 1])
			ft_putstr_fd(" ", temp);
		i++;
	}
	if (n_flag)
		ft_putstr_fd("\n", 1);
	return (0);
}
