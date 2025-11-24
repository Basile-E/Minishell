/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_mgmt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:12:20 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 20:12:21 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*create_and_add_token(t_token **head, t_token **current, char *content)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->value = ft_strdup(content);
	new_node->next = NULL;
	if (!*head)
	{
		*head = new_node;
		*current = new_node;
	}
	else
	{
		(*current)->next = new_node;
		*current = new_node;
	}
	return (new_node);
}
void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}
void	crea_tok_pipe(int *i, t_token **tokens)
{
	t_token	*token;

	token = create_token(PIPE, "|");
	*i += 1;
	add_token(tokens, token);
}

void	crea_tok_app(char *input, int *i, t_token **tokens)
{
	t_token	*token;

	if (input[*i + 1] && input[*i + 1] == '>')
	{
		token = create_token(REDIRECT_APPEND, ">>");
		*i += 2;
	}
	else
	{
		token = create_token(REDIRECT_OUT, ">");
		*i += 1;
	}
	add_token(tokens, token);
}

void	crea_tok_her(char *input, int *i, t_token **tokens)
{
	t_token	*token;

	if (input[*i + 1] && input[*i + 1] == '<')
	{
		token = create_token(REDIRECT_HEREDOC, "<<");
		*i += 2;
	}
	else
	{
		token = create_token(REDIRECT_IN, "<");
		*i += 1;
	}
	add_token(tokens, token);
}
