#include "minishell.h"

int is_a_builtin(char **cmd, t_minishell *mini)
{

	//if (ft_strncmp("cd", cmd, ft_strlen(cmd) + 1))
	if (ft_strncmp("echo", cmd[0], 4) == 0 && ft_strlen(cmd[0]) == 4)
		ft_echo(*mini, cmd);

	//if (ft_strncmp("exit", cmd, ft_strlen(cmd) + 1))

	//if (ft_strncmp("export", cmd, ft_strlen(cmd) + 1))

	//if (ft_strncmp("pwd", cmd, ft_strlen(cmd) + 1))

	//if (ft_strncmp("unset", cmd, ft_strlen(cmd) + 1))
	return (1);
}


int execute(t_cmd *cmd, t_minishell *mini)
{

	t_cmd *current;
	char *cmd_name;
	(void) cmd_name;
	current = cmd;
	while(current)
	{
		is_a_builtin(current->args, mini);
		current = current->next;
	}
	return(1);
}