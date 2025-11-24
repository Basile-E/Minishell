/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_type.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 21:58:21 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 20:58:46 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_type_word(t_lexer *lexer, t_token **current)
{
	if (!lexer->cmd_tab)
	{
		lexer->words = count_words_until_pipe(*current);
		lexer->cmd_tab = ft_calloc(lexer->words + 1, sizeof(char *));
		if (!lexer->cmd_tab)
			return (0);
		lexer->tab_idx = 0;
	}
	lexer->cmd_tab[lexer->tab_idx++] = ft_strdup((*current)->value);
	*current = (*current)->next;
	return (1);
}

int	do_type_pipe(t_token **current, t_cmd **new_node, t_lexer *lexer,
		t_cmd **cmd)
{
	(*new_node) = cmd_create(lexer);
	if (!(*new_node))
		return (0);
	(*new_node)->heredoc = lexer->heredoc;
	lexer->heredoc = NULL;
	cmd_append(cmd, (*new_node));
	lexer->cmd_tab = NULL;
	lexer->tab_idx = 0;
	lexer->fd_in = -1;
	lexer->fd_out = -1;
	lexer->app_mode = 0;
	(*current) = (*current)->next;
	return (1);
}

int	do_type_redirout(t_lexer *lexer, t_token **current)
{
	printf("DEBUG REDIR_OUT: file='%s'\n", (*current)->next->value);
	if (lexer->fd_out != -1)
		close(lexer->fd_out);
	lexer->fd_out = open((*current)->next->value, O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	printf("DEBUG REDIR_OUT: fd_out=%d\n", lexer->fd_out);
	if (lexer->fd_out == -1)
		return (perror("open"), 0);
	*current = (*current)->next->next;
	return (1);
}

int	do_type_redirapp(t_lexer *lexer, t_token **current)
{
	if (lexer->fd_out != -1)
		close(lexer->fd_out);
	lexer->fd_out = open((*current)->next->value, O_WRONLY | O_CREAT | O_APPEND,
			0644);
	if (lexer->fd_out == -1)
		return (perror("open"), 0);
	*current = (*current)->next->next;
	return (1);
}

int	do_type_redirher(t_lexer *lexer, t_token **current)
{
	if (lexer->heredoc)
	{
		free(lexer->heredoc);
		lexer->heredoc = NULL;
	}
	if (do_heredoc((*current)->next->value, &lexer->heredoc) == -1)
		return (0);
	*current = (*current)->next->next;
	return (1);
}
