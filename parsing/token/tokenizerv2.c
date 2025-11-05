#include "../includes/minishell.h"

// Vérifie si le caractère est un opérateur
// int	is_operator(char c)
// {
//     return (c == '|' || c == '<' || c == '>');
// }

// Vérifie si le caractère est un whitespace
// int	is_whitespace(char c)
// {
//     return (c == ' ' || c == '\t' || c == '\n');
// }

// Crée un nouveau token
t_token	*create_token(t_token_type type, char *value)
{
    t_token	*token;

    token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->value = ft_strdup(value);
    token->next = NULL;
    return (token);
}

// Ajoute un token à la liste
void	add_token(t_token **head, t_token *new_token)
{
    t_token	*current;

    if (!*head)
    {
        *head = new_token;
        return;
    }
    current = *head;
    while (current->next)
        current = current->next;
    current->next = new_token;
}

// Parse les opérateurs de redirection
int	parse_redirect_operator(char *input, int *i, t_token **tokens)
{
    t_token	*token;

    if (input[*i] == '<')
    {
        if (input[*i + 1] == '<')
        {
            token = create_token(REDIRECT_HEREDOC, "<<");
            *i += 2;
        }
        else
        {
            token = create_token(REDIRECT_IN, "<");
            *i += 1;
        }
    }
    else if (input[*i] == '>')
    {
        if (input[*i + 1] == '>')
        {
            token = create_token(REDIRECT_APPEND, ">>");
            *i += 2;
        }
        else
        {
            token = create_token(REDIRECT_OUT, ">");
            *i += 1;
        }
    }
    else if (input[*i] == '|')
    {
        token = create_token(PIPE, "|");
        *i += 1;
    }
    else
        return (0);
    
    add_token(tokens, token);
    return (1);
}

// Parse un mot (word token)
int	parse_word(char *input, int *i, t_token **tokens, t_quote status)
{
    int		start;
    int		len;
    char	*word;
    t_token	*token;

    start = *i;
    while (input[*i] && (( !is_space(input[*i]) && !is_operator(input[*i])) || status != NONE))
        (*i)++;
    
    len = *i - start;
    word = ft_substr(input, start, len);
    token = create_token(WORD, word);
    free(word);
    add_token(tokens, token);
    return (1);
}

// Fonction principale de tokenisation
t_token	*tokenize(char *input)
{
	t_quote status;
    t_token	*tokens;
    int		i;


	status = NONE;
    tokens = NULL;
    i = 0;
    
    while (input[i])
    {
		set_quote_status(input[i], &status);
        // Skip whitespace
        while (input[i] && is_space(input[i]) && status == NONE)
            i++;
        
        if (!input[i])
            break;
        
        // Parse operators
        if (is_operator(input[i]) && status == NONE)
        {
            if (!parse_redirect_operator(input, &i, &tokens))
                return (NULL);
        }
        // Parse words
        else
        {
            if (!parse_word(input, &i, &tokens, status))
                return (NULL);
        }
    }
    
    return (tokens);
}

// Libère la liste de tokens
void	free_tokens(t_token *tokens)
{
    t_token	*current;
    t_token	*next;

    current = tokens;
    while (current)
    {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
}