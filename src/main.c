#include "../includes/minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	main(int ac, char **av, char **ev)
{
	char		*prompt;
	t_minishell	*minishell;
	int			ret;

	(void)ac;
	(void)av;
	minishell = malloc(sizeof(t_minishell));
	prompt = "\001\033[1;36m\002RicoShell \001\033[0;95m\002\001➜\002 \001\033[0m\002";
	set_struct_minishell(minishell, ev);
	while (42)
	{
		if (minishell->status == KILL_SIM)
			break ;
		minishell->input = readline(prompt);
		if (!minishell->input)
			return (printf("Exit\n"), 0);
		add_history(minishell->input);
		ret = parsinette(&minishell);
		if (ret == -1)
			printf("Parsing failed, try to be better at cli\n");
	}
	return (0);
}
