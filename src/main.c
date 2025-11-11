#include "../includes/minishell.h"

int main(int ac, char **av, char **ev)
{
    (void) ac;
    (void) av;

    char *prompt;
    t_minishell *minishell;

    minishell = malloc(sizeof(t_minishell));
    prompt = BOLD CYAN "RicoShell" RESET PINK " ➜ " RESET;
    
    set_struct_minishell(minishell, ev); //ici remplir env 

    while (42)
    {
        if (minishell->status == KILL_SIM)
            break;

        minishell->input = readline(prompt);
        if (!minishell->input) // est-ce que readline peut fail ?
            return(printf("Exit\n"), 0);
        add_history(minishell->input);
        //printf("Debug :\nString sent by readline : %s\n", minishell->input);
        if (parsinette(minishell))
            printf("Parsing failed, try to be better at cli\n");
    }
    return(0);
}
