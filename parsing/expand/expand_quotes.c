/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emle-vou <emle-vou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 22:33:00 by emle-vou          #+#    #+#             */
/*   Updated: 2025/11/23 22:35:21 by emle-vou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_quote_status(char c, t_quote *status)
{
	if (*status == NONE && c == '\'')
		*status = SINGLE;
	else if (*status == NONE && c == '"')
		*status = DOUBLE;
	else if ((*status == SINGLE && c == '\'') || (*status == DOUBLE
			&& c == '"'))
		*status = NONE;
}

void	set_quote(t_r_quote *r_quote, char *str)
{
	r_quote->i = 0;
	r_quote->j = 0;
	r_quote->len = ft_strlen(str);
	r_quote->status = NONE;
	r_quote->quote_char = '\0';
	r_quote->result = malloc(r_quote->len + 1);
}

char	*remove_quotes(char *str)
{
	t_r_quote	r_quote;

	set_quote(&r_quote, str);
	if (!r_quote.result)
		return (NULL);
	while (str[r_quote.i++])
	{
		if (r_quote.status == NONE && (str[r_quote.i] == '\''
				|| str[r_quote.i] == '"'))
		{
			r_quote.quote_char = str[r_quote.i];
			r_quote.status = (str[r_quote.i] == '\'') ? SINGLE : DOUBLE;
		}
		else if (r_quote.status != NONE && str[r_quote.i] == r_quote.quote_char)
		{
			r_quote.status = NONE;
			r_quote.quote_char = '\0';
		}
		else
			r_quote.result[r_quote.j++] = str[r_quote.i];
	}
	if (str)
		free(str);
	r_quote.result[r_quote.j] = '\0';
	return (r_quote.result);
}
