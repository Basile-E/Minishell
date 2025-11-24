/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:56:11 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 20:56:38 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_alloc(t_alloc **head, t_alloc *new_token)
{
	t_alloc	*current;

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

t_alloc	*create_alloc(void *adr)
{
	t_alloc	*alloc;

	alloc = malloc(sizeof(t_alloc));
	if (!alloc)
		return (NULL);
	alloc->adr = adr;
	alloc->next = NULL;
	return (alloc);
}

void	*ft_malloc(int size, t_alloc **head)
{
	void	*adr;
	t_alloc	*node;

	adr = malloc(size);
	if (!adr)
		return (NULL);
	node = create_alloc(adr);
	if (!node)
	{
		free(adr);
		return (NULL);
	}
	add_alloc(head, node);
	return (adr);
}

void	free_alloc(t_alloc *head)
{
	t_alloc	*current;
	t_alloc	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current->adr);
		free(current);
		current = next;
	}
}
