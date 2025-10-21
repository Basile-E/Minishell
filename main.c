#include "minishell.h"


int set_struct(t_minishell *minishell)
{
    minishell->input = NULL;
    minishell->status = DONT_KILL;
    return (1);
}


int main()
{
    t_minishell *minishell;
    minishell = malloc(sizeof(t_minishell));
    set_struct(minishell);

    while (42)
    {
        if (minishell->status == KILL_SIM)
            break;

        minishell->input = readline("RicoShell > ");
        if (!minishell->input) // est-ce que readline peut fail ?
            return(printf("Readline Failed\n"), 1);
        add_history(minishell->input);
        printf("Debug :\nString sent to readline : %s\n", minishell->input);
        printf("\n");
        if (!ft_strncmp(minishell->input, "kill", 4))
            minishell->status = KILL_SIM;
    }
    return(0);
}


// compiling cmd : cc -g3 -Wall -Werror -Wextra main.c -L./Libft -lft -lreadline -o RicoShell