/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 19:50:55 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 19:54:46 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	list_expand(t_minishell *minishell, t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (current->type == WORD)
		{
			process_word_token(minishell, current);
			do_field_spliting(current);
		}
		current = current->next;
	}
	return (1);
}
