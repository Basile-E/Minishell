#include "../includes/minishell.h"

t_token *create_and_add_token(t_token **head, t_token **current, char *content)
{
    t_token *new_node = malloc(sizeof(t_token));
    if (!new_node)
        return (NULL);
    
    new_node->value = ft_strdup(content);
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
