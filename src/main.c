#include "../includes/minishell.h"

void	exit_minishell(t_minishell* mini)
{
	if (mini->input)
		free(mini->input);
	rl_clear_history();
	free(mini);
	exit(0);
}

void	free_each(t_minishell *mini)
{
	free(mini->input);
}

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
    	minishell->input = readline(prompt);
       if (!minishell->input) // est-ce que readline peut fail ?
	   {
			printf("exit\n");
            exit_minishell(minishell);
	   }
        add_history(minishell->input);
        //printf("Debug :\nString sent by readline : %s\n", minishell->input);
        if (parsinette(minishell))
            printf("Parsing failed, try to be better at cli\n");
		free_each(minishell);
    }
    return(0);
}
