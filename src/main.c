#include "../includes/minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	main(int ac, char **av, char **ev)
{
	char		*prompt;
	t_minishell	*minishell;
	// int			pars_ret;

	(void)ac;
	(void)av;
	minishell = malloc(sizeof(t_minishell));
	prompt = "\001\033[1;36m\002RicoShell \001\033[0;95m\002\001➜\002 \001\033[0m\002";
	set_struct_minishell(minishell, ev); // ici remplir env
	while (42)
	{
		if (minishell->status == KILL_SIM)
			break ;
		minishell->input = readline(prompt);
		if (!minishell->input) // ctrl d
			return (printf("Exit\n"), 0);
		add_history(minishell->input);
		// printf("Debug :\nString sent by readline : %s\n", minishell->input);
		parsinette(&minishell);
		free_alloc(minishell->alloc);
	}
	return (0);
}
