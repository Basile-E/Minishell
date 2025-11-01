#include "../includes/minishell.h"

int main(int ac, char **av, char **ev)
{
    char *prompt;
    t_minishell *minishell;

    minishell = malloc(sizeof(t_minishell));
    prompt = BOLD CYAN "RicoShell" RESET PINK " ➜ " RESET;
    
    set_struct_minishell(minishell, ev);
    
    // mode headless
    if (ac != 1)
    {
        if (ac == 3 && (ft_strncmp(av[1], "-i", 2) == 0))
        {
            minishell->input = av[2];
            parsinette(minishell);
            //print_token(args); // sert a debug
        }
        return (0);
    }

    // mode exec
    while (42)
    {
        if (minishell->status == KILL_SIM)
            break;

        minishell->input = readline(prompt);
        if (!minishell->input) // est-ce que readline peut fail ?
            return(printf("Exit\n"), 0);
        add_history(minishell->input);
        printf("Debug :\nString sent by readline : %s\n", minishell->input);
        // if (!tokenizer(minishell, &args))
        //     printf("Tokenizer Failed\n"); // le tout est un peut sale mais c'est une base pour gerer les retours d'erreurs (plusieurs edges cases a gerer : user sent \n -> on print le retour a la ligne (arg sera == a \n), user sent too much char ou une cmd illegal -> print un msg d'erreur standard type cmd illegal cmd name to long)
        // else
        //     printf("\n");
        parsinette(minishell);
        //print_token(args); // sert a debug
    }
    return(0);
}
