/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 21:45:45 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/23 23:14:50 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_set(t_cmd *new, int fd_in, int fd_out, int app_mode)
{
	new->fd_in = fd_in;
	new->fd_out = fd_out;
	new->append_mode = app_mode;
	new->next = NULL;
	new->heredoc = NULL;
	new->in_child = 0;
}

t_cmd	*cmd_create(char **cmds, int fd_in, int fd_out, int app_mode)
{
	t_cmd	*new;
	int		i;
	int		count;

	if (!cmds)
		return (NULL);
	count = 0;
	while (cmds[count])
		count++;
	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->args = ft_calloc(count + 1, sizeof(char *));
	if (!new->args)
	{
		free(new);
		return (NULL);
	}
	i = -1;
	while (++i < count)
		new->args[i] = ft_strdup(cmds[i]);
	cmd_set(new, fd_in, fd_out, app_mode);
	new->args[i] = NULL;
	free(cmds);
	return (new);
}

void	cmd_append(t_cmd **cmd, t_cmd *new_node)
{
	t_cmd	*current;

	if (!*cmd)
	{
		*cmd = new_node;
		return ;
	}
	current = *cmd;
	while (current->next)
		current = current->next;
	current->next = new_node;
}
