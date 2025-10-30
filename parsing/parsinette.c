#include "../includes/minishell.h"

int parsinette(t_minishell *minishell, t_args **args)
{
	t_args *current;

	current = *args;
	while (current)
	{
		current->arg = do_expand(minishell, current->arg);
		
		current = current->next;
	}
	return (0);
}