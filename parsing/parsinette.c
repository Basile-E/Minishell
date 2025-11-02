#include "../includes/minishell.h"

int process_word_token(t_minishell *minishell, t_token *token)
{
    char *expanded;
    char *cleaned;
    
    expanded = do_expand_simple(minishell, token->value);

    cleaned = remove_quotes(expanded);

    free(token->value);
    token->value = cleaned;
    
    free(expanded);
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

int	list_expand(t_minishell *minishell, t_token *token)
{
	t_token *current;

	current = token;
	while (current)
    {
        if (current->type == WORD)
            process_word_token(minishell, current);
        
		// here can go other thing like expand or string modification based on the type of the token
        current = current->next;
    }
    print_list_type_debug(token);
	return (1);
}

int parsinette(t_minishell *minishell)
{
    t_token *tokens;

	if	(check_parentheses_syntax(minishell->input) || 
		check_unclosed_quotes(minishell->input))
		return (1);

    tokens = tokenize(minishell->input);
    if (!tokens)
	{
        return (1);
	} // wtf a cause du print je dois foutre des crochets 
	
	print_token(tokens); // debug

    if(!list_expand(minishell, tokens))
		return (1);
    
	if(!check_syntax_errors(tokens))
		return (1);

    return (0);
}



//     echo hello | | world → pipes consécutifs DONE
//     echo hello > > file → redirections consécutives DONNE
//     echo hello > → redirection sans fichier DONE
//     echo hello | → pipe en fin de ligne DONE
// |   echo hello → pipe en début de ligne DONE
//     echo hello | | world → commande vide après pipe DONE