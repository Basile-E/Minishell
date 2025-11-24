/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 21:45:45 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 20:57:41 by baecoliv         ###   ########.fr       */
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

void	free_string_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

t_cmd	*cmd_create(t_lexer *lexer)
{
	t_cmd	*new;
	int		i;
	int		count;

	if (!lexer->cmd_tab)
		return (NULL);
	count = 0;
	while (lexer->cmd_tab[count])
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
		new->args[i] = ft_strdup(lexer->cmd_tab[i]);
	cmd_set(new, lexer->fd_in, lexer->fd_out, lexer->app_mode);
	free_string_array(lexer->cmd_tab);
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
