#include "../includes/minishell.h"

t_token	*create_and_add_token(t_token **head, t_token **current, char *content)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->value = ft_strdup_gc(content);
	new_node->next = NULL;
	if (!*head)
	{
		*head = new_node;
		*current = new_node;
	}
	else
	{
		(*current)->next = new_node;
		*current = new_node;
	}
	return (new_node);
}
