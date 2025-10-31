#include "../includes/minishell.h"

int parsinette(t_minishell *minishell, t_args **args)
{
	t_args *current;

	current = *args;
	while (current)
	{
		
		//il faut un quote cleaner ou fix mon tokenizer pour qu'il vire les quotes
		current->arg = do_expandV2(minishell, current->arg);
		
		
		current = current->next;
	}
	return (0);
}