#include "../includes/minishell.h"

// Nouvelle approche : parser directement depuis l'input
void process_word_token(t_minishell *minishell, t_token *token)
{
    char *expanded;
    char *cleaned;
    
    // 1. Expansion des variables
    expanded = do_expand_simple(minishell, token->value);
    
    // 2. Nettoyage des quotes
    cleaned = remove_quotes(expanded);
    
    // 3. Remplacement de la valeur
    free(token->value);
    token->value = cleaned;
    
    free(expanded);
}

int parsinette(t_minishell *minishell)
{
    t_token *tokens;
    t_token *current;

    // Tokenizer l'input complet
    tokens = tokenize(minishell->input);
    if (!tokens)
        return (1);
    print_token(tokens);

    // Parcourir les tokens et les traiter
    current = tokens;
    while (current)
    {
        if (current->type == WORD)
        {
            process_word_token(minishell, current);
            printf("Processed word: %s\n", current->value);
        }
        else if (current->type == PIPE)
        {
            printf("Pipe found: %s\n", current->value);
        }
        else if (current->type == REDIRECT_OUT)
        {
            printf("Redirect out: %s\n", current->value);
        }
		else if (current->type == REDIRECT_APPEND)
		{
			printf("Redirect Append found : %s\n", current->value);
		}
		else if (current->type == REDIRECT_HEREDOC)
		{
			printf("Redirect Heredoc found : %s\n", current->value);
		}
        // ... autres types
        

		/*
			fin debug des types, a ce moment j'ai une liste de token avec tout les expand fait
			je devrait probablement clean les quotes ?
			ensuite je dois faire le parsing de la syntaxe
			
		*/


        current = current->next;
    }
    

    free_tokens(tokens);
    return (0);
}
