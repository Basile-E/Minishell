#include "../includes/minishell.h"

int check_parentheses_syntax(char *input)
{
    int i = 0;
    t_quote status = NONE;
    
    while (input[i])
    {
        set_quote_status(input[i], &status);
        
        // Vérifier les parenthèses seulement hors des quotes
        if ((input[i] == '(' || input[i] == ')') && status == NONE)
        {
            // Parenthèses détectées hors quotes = erreur de syntaxe
            printf("bash: erreur de syntaxe près du symbole inattendu « %c »\n", input[i]);
            return (1); // Erreur
        }
        i++;
    }
    return (0); // OK
}

int check_syntax_errors(char *input)
{
    // Vérifier différents types d'erreurs de syntaxe
    if (check_parentheses_syntax(input))
        return (1);
    
    // Vous pouvez ajouter d'autres vérifications ici :
    // - Pipes en début/fin de ligne
    // - Redirections sans fichier
    // - etc.
    
    return (0);
}