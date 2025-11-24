/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: basile <basile@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:15:05 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 05:30:21 by basile           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* I know a lexer is a tokenizer, go touch grass */

int	count_words_until_pipe(t_token *start)
{
	int	count;

	count = 0;
	while (start && start->type != PIPE)
	{
		if (start->type == WORD)
			count++;
		start = start->next;
	}
	return (count);
}

int	do_type_redirout(t_lexer *lexer, t_token **current)
{
	lexer->fd_out = open((*current)->next->value, O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	if (lexer->fd_out == -1)
		return (0);
	*current = (*current)->next->next;
	return (1);
}

void free_token(t_token *token)
{
	t_token *next;
	
	while (token)
	{
		next = token->next;
		free(token);
		token = next;		
	}
}

t_cmd	*lexer(t_token *token, t_cmd *cmd, t_minishell *mini)
{
	t_token	*current;
	t_lexer	*lexer;
	t_cmd	*new_node;

	current = token;
	set_lexer(&lexer, mini);
	while (current)
		if (!lexinette(lexer, &current, &new_node, &cmd))
			return (NULL);
	free_token(token);
	if (lexer->cmd_tab)
		do_lexing(lexer, &new_node, &cmd, mini);
	return (cmd);
}
