/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsinette.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 19:35:28 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 19:54:30 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	parsinette(t_minishell **minishell)
{
	t_token	*tokens;
	t_cmd	*cmd;

	if (check_parentheses_syntax((*minishell)->input)
		|| check_unclosed_quotes((*minishell)->input))
		return (1);
	if (!(*minishell)->input[0])
		return (0);
	cmd = NULL;
	tokens = tokenize((*minishell)->input);
	if (!tokens)
		return (1);
	if (!list_expand(*minishell, tokens))
		return (-1);
	if (!do_field_spliting(tokens))
		return (-1);
	if (!remove_all_quote(tokens))
		return (-1);
	if (!check_syntax_errors(tokens))
		return (-1);
	cmd = lexer(tokens, cmd, *minishell);
	if (!cmd)
		return (1);
	return (execute(cmd, *minishell));
}
