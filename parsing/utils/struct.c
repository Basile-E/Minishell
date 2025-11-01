#include "../includes/minishell.h"

int set_struct_minishell(t_minishell *minishell, char **env)
{
    minishell->input = NULL;
    minishell->status = DONT_KILL;
    minishell->env = env;
    return (1);
}