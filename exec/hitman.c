/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hitman.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 22:03:34 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 22:23:16 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute(t_cmd *cmd, t_minishell *mini)
{
	if (!cmd)
		return (0);
	if (cmd->next)
		exec_mult(cmd, mini);
	else
	{
		if (!is_a_builtin(cmd, mini))
			exec_single(cmd, mini);
	}
	free_cmd(cmd);
	return (mini->status);
}
