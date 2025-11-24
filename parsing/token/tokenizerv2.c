/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizerv2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:12:03 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 20:12:06 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	set_var(t_quote *status, t_token **tokens, int *i)
{
	*status = NONE;
	*tokens = NULL;
	*i = 0;
}

t_token	*tokenize(char *input)
{
	t_quote	status;
	t_token	*tokens;
	int		i;

	set_var(&status, &tokens, &i);
	while (input[i])
	{
		set_quote_status(input[i], &status);
		while (input[i] && is_space(input[i]) && status == NONE)
			i++;
		set_quote_status(input[i], &status);
		if (!input[i])
			break ;
		if (is_operator(input[i]) && status == NONE)
		{
			if (!parse_redirect_operator(input, &i, &tokens))
				return (NULL);
		}
		else
		{
			if (!parse_word(input, &i, &tokens, &status))
				return (NULL);
		}
	}
	return (free(input), tokens);
}
