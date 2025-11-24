/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 21:55:20 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 21:17:47 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_lexer(t_lexer **lexer, t_minishell *mini)
{
	*lexer = ft_malloc(sizeof(t_lexer), &mini->alloc);
	(*lexer)->mini = &mini;
	(*lexer)->cmd_tab = NULL;
	(*lexer)->tab_idx = 0;
	(*lexer)->app_mode = 0;
	(*lexer)->fd_in = -1;
	(*lexer)->fd_out = -1;
	(*lexer)->ret_val = 1;
	(*lexer)->heredoc = NULL;
}

int	do_lexing(t_lexer *lexer, t_cmd **new_node, t_cmd **cmd)
{
	(*new_node) = cmd_create(lexer);
	if (!*new_node)
		return (0);
	(*new_node)->heredoc = lexer->heredoc;
	cmd_append(cmd, *new_node);
	return (1);
}

int	lexinette(t_lexer *lexer, t_token **current, t_cmd **new_node, t_cmd **cmd)
{
	if ((*current)->type == WORD)
		lexer->ret_val = do_type_word(lexer, current);
	else if (!(*current)->next)
		return (0);
	else if ((*current)->type == REDIRECT_APPEND)
		lexer->ret_val = do_type_redirapp(lexer, current);
	else if ((*current)->type == REDIRECT_HEREDOC)
		lexer->ret_val = do_type_redirher(lexer, current);
	else if ((*current)->type == REDIRECT_IN)
		lexer->ret_val = do_type_redirin(lexer, current);
	else if ((*current)->type == REDIRECT_OUT)
		lexer->ret_val = do_type_redirout(lexer, current);
	else if ((*current)->type == PIPE)
		lexer->ret_val = do_type_pipe(current, new_node, lexer, cmd);
	else
		*current = (*current)->next;
	return (lexer->ret_val);
}

void	print_lexer(t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		print_env(current->args);
		printf("%i\n", current->fd_in);
		printf("%i\n", current->fd_out);
		printf("%i\n", current->append_mode);
		if (current->heredoc)
			printf("%s\n", current->heredoc);
		current = current->next;
	}
}
