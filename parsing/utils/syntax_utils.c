/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 19:53:09 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 19:55:07 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_first_arg_is_flag(char *input)
{
	if (input[0] == '-')
		return (1);
	return (0);
}

int	get_list_len(t_token *head)
{
	int		len;
	t_token	*current;

	len = 0;
	current = head;
	while (current)
	{
		len++;
		current = current->next;
	}
	return (len);
}

int	check_last_pipe(t_token *head)
{
	t_token	*current;

	current = head;
	while (current->next)
		current = current->next;
	if (current->value[0] == '|')
		return (1);
	return (0);
}

int	check_first_pipe(t_token *head)
{
	if (head->value[0] == '|')
		return (1);
	return (0);
}

int	check_pipes_first_or_last(t_token *head)
{
	if (check_first_pipe(head) || check_last_pipe(head))
		return (1);
	return (0);
}
