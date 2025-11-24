/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 19:45:08 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 19:56:33 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_list_type_debug(t_token *head)
{
	t_token	*current;

	current = head;
	while (current)
	{
		if (current->type == WORD)
			printf("Processed word: %s\n", current->value);
		else if (current->type == PIPE)
			printf("Pipe found: %s\n", current->value);
		else if (current->type == REDIRECT_OUT)
			printf("Redirect out: %s\n", current->value);
		else if (current->type == REDIRECT_APPEND)
			printf("Redirect Append found : %s\n", current->value);
		else if (current->type == REDIRECT_HEREDOC)
			printf("Redirect Heredoc found : %s\n", current->value);
		current = current->next;
	}
}
