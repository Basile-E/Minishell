/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_split_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 19:49:50 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 20:00:32 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_field_spliting(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		current = split_and_insert(current);
		if (!current)
			return (0);
		current = current->next;
	}
	return (1);
}

int	remove_all_quote(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		current->value = remove_quotes(current->value);
		current = current->next;
	}
	return (1);
}
