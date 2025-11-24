/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:10:42 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 20:12:31 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

int	parse_redirect_operator(char *input, int *i, t_token **tokens)
{
	if (input[*i] == '<')
	{
		crea_tok_her(input, i, tokens);
	}
	else if (input[*i] == '>')
	{
		crea_tok_app(input, i, tokens);
	}
	else if (input[*i] == '|')
	{
		crea_tok_pipe(i, tokens);
	}
	else
		return (0);
	return (1);
}

int	parse_word(char *input, int *i, t_token **tokens, t_quote *status)
{
	int		start;
	int		len;
	char	*word;
	t_token	*token;

	start = *i;
	while (input[*i] && ((!is_space(input[*i]) && !is_operator(input[*i]))
			|| *status != NONE))
	{
		set_quote_status(input[*i], status);
		(*i)++;
	}
	len = *i - start;
	word = ft_substr(input, start, len);
	token = create_token(WORD, word);
	free(word);
	add_token(tokens, token);
	return (1);
}
