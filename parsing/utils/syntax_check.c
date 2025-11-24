/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 19:55:39 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 19:55:47 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_parentheses_syntax(char *input)
{
	int		i;
	t_quote	status;

	i = 0;
	status = NONE;
	while (input[i])
	{
		set_quote_status(input[i], &status);
		if ((input[i] == '(' || input[i] == ')') && status == NONE)
		{
			printf("bash: erreur de syntaxe près du symbole inattendu « %c »\n",
				input[i]);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_bad_redir_syntax(t_token *current)
{
	if (!current->next)
		return (1);
	if ((current->type == REDIRECT_IN && current->next->type == REDIRECT_OUT)
		|| (current->type == REDIRECT_OUT
			&& current->next->type == REDIRECT_IN))
		return (1);
	if (current->next->type != WORD)
		return (1);
	return (0);
}

int	check_syntax_errors(t_token *head)
{
	t_token	*current;

	current = head;
	if (check_first_arg_is_flag(head->value) || check_pipes_first_or_last(head))
		return (0);
	while (current)
	{
		if (current->type == REDIRECT_APPEND
			|| current->type == REDIRECT_HEREDOC || current->type == REDIRECT_IN
			|| current->type == REDIRECT_OUT)
		{
			if (check_bad_redir_syntax(current))
				return (0);
		}
		current = current->next;
	}
	return (1);
}
