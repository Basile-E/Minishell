#include "minishell.h"

int check_for_builtin(char **cmd)
{
	if (!ft_strncmp("echo", *cmd, ft_strlen(*cmd) + 1))
		return (1);
	if (!ft_strncmp("exit", *cmd, ft_strlen(*cmd) + 1))
		return (1);
	if (!ft_strncmp("export", *cmd, ft_strlen(*cmd) + 1))
		return (1);
	if (!ft_strncmp("pwd", *cmd, ft_strlen(*cmd) + 1))
		return (1);
	if (!ft_strncmp("env", *cmd, ft_strlen(*cmd) + 1))
		return (1);
	return (0);
}

int is_a_builtin(char **cmd, t_minishell *mini, int in_child)
{
	if (check_for_builtin(cmd))
	{
		//if (ft_strncmp("cd", cmd, ft_strlen(cmd) + 1))
		if (!ft_strncmp("echo", cmd[0], 4) && ft_strlen(cmd[0]) == 4)
			ft_echo(*mini, cmd);
		if (!ft_strncmp("exit", *cmd, ft_strlen(*cmd) + 1))
			ft_exit(cmd, mini, in_child);
		if (!ft_strncmp("export", *cmd, ft_strlen(*cmd) + 1))
			ft_export(mini, cmd);
		if (!ft_strncmp("pwd", *cmd, ft_strlen(*cmd) + 1))
			ft_pwd();
		if (!ft_strncmp("env", *cmd, ft_strlen(*cmd) + 1))
			ft_env(mini);
		// if (ft_strncmp("unset", *cmd, ft_strlen(*cmd) + 1))
		// 	ft_unset();
	}
	return (0);
}


int execute(t_cmd *cmd, t_minishell *mini)
{

	t_cmd *current;
	char *cmd_name;
	(void) cmd_name;
	current = cmd;
	while(current)
	{
		is_a_builtin(current->args, mini, current->in_child);
		current = current->next;
	}
	return(1);
}