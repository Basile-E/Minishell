/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: basile <basile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 21:45:45 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 05:49:41 by basile           ###   ########.fr       */
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

t_cmd	*cmd_create(t_lexer *lexer, t_minishell *mini)
{
	t_cmd	*new;
	int		i;
	int		count;

	if (!lexer->cmd_tab)
		return (NULL);
	count = 0;
	while (lexer->cmd_tab[count])
		count++;
	new = ft_malloc(sizeof(t_cmd), &mini->alloc);
	if (!new)
		return (NULL);
	new->args = ft_calloc_gc(count + 1, sizeof(char *), mini);
	if (!new->args)
	{
		free(new);
		return (NULL);
	}
	i = -1;
	while (++i < count)
		new->args[i] = ft_strdup_gc(lexer->cmd_tab[i], mini);
	cmd_set(new, lexer->fd_in, lexer->fd_out, lexer->app_mode);
	new->args[i] = NULL;
	free(lexer->cmd_tab);
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
