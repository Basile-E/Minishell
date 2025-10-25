#include "../includes/minishell.h"

t_args *create_and_add_token(t_args **head, t_args **current, char *content)
{
    t_args *new_node = malloc(sizeof(t_args));
    if (!new_node)
        return (NULL);
    
    new_node->arg = ft_strdup(content);
    new_node->next = NULL;
    
    // Si c'est le premier nœud
    if (!*head)
    {
        *head = new_node;
        *current = new_node;
    }
    else
    {
        // Ajouter à la fin de la liste
        (*current)->next = new_node;
        *current = new_node;
    }
    
    return (new_node);
}