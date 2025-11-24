#include "../includes/minishell.h"

int process_word_token(t_minishell *minishell, t_token *token)
{
    char *expanded;
    char *cleaned;

    expanded = do_expand_simple(minishell, token->value);
    (void)cleaned;


    free(token->value);
    //token->value = cleaned;
    token->value = expanded;
    // free(expanded); // <-- ERROR: Do not free the string you just assigned.
	return(1);
}

void	print_list_type_debug(t_token *head)
{
	t_token *current;
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

t_token	*tkn_new(char *str)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return  NULL;
	new->value = str;
	new->next = NULL;
	return (new);
}


void	tkn_append_after( t_token *current,  t_token *add)
{
	add->next = current->next;
	current->next = add;
}

static t_token	*split_and_insert(t_token *current)
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

int	should_expand_wildcards(char *value)
{
    int i = 0;
    t_quote status;
    
	status = NONE;
    while (value[i])
    {
        set_quote_status(value[i], &status);
        if (value[i] == '*' && status != NONE)
            return (0); // Don't expand if * is quoted
        i++;
    }
    return (1); // OK to expand
}

int	list_expand(t_minishell *minishell, t_token *token)
{
	t_token *current;

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

int	remove_all_quote(t_token *tokens)
{
	t_token *current;

	current = tokens;
	while(current)
	{
		current->value = remove_quotes(current->value);
		current = current->next;
	}
	return (1);
}


int parsinette(t_minishell **minishell)
{
    t_token *tokens;
	t_cmd 	*cmd;

	if	(check_parentheses_syntax((*minishell)->input) || 
		check_unclosed_quotes((*minishell)->input))
		return (1);
	if (!(*minishell)->input[0])
		return(0);
	cmd = NULL;
    tokens = tokenize((*minishell)->input);
	if (!tokens)
        return (1);
    if(!list_expand(*minishell, tokens))
		return (-1);
	if (!do_field_spliting(tokens))
		return (-1);
	if (!remove_all_quote(tokens))
		return (-1);
	if (!check_syntax_errors(tokens))
		return (-1);
	cmd = lexer(tokens, cmd, *minishell);
	if (!cmd)
		return (1);
    return (execute(cmd, *minishell));
}
