/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tkn_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 19:44:17 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 19:55:09 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*tkn_new(char *str)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = str;
	new->next = NULL;
	return (new);
}

void	tkn_append_after(t_token *current, t_token *add)
{
	add->next = current->next;
	current->next = add;
}

t_token	*split_and_insert(t_token *current)
{
	char	**fields;
	int		i;

	if (!current->value || !ft_strchr(current->value, ' '))
		return (current);
	fields = ft_split(current->value, ' ');
	if (!fields)
		return (NULL);
	free(current->value);
	current->value = fields[0];
	i = 1;
	while (fields[i])
	{
		tkn_append_after(current, tkn_new(fields[i]));
		current->next->type = WORD;
		current = current->next;
		i++;
	}
	free(fields);
	return (current);
}
