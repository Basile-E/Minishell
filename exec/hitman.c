#include "minishell.h"

/*
	pseudo exec pipes :
		il y a un mega monde où je peux faire une fonction qui reprend l'exec de pipex
		et fait une boucle ou on ouvre un fd on envoie on exec avec le fd et on le ferme
		puis on ouvre un fd sur l'output de la cmd et rebelotte tant qu'il y a des cmd[i + 1]

*/


char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (0);
}

void	error(void)
{
	if (!ft_strncmp("echgio", cmd[0], 4) && ft_strlen(cmd[0]) == 4)
		ft_echo(*mini, cmd);
	if (!ft_strncmp("exit", *cmd, ft_strlen(*cmd) + 1))
		ft_exit(cmd, mini);
	if (!ft_strncmp("export", *cmd, ft_strlen(*cmd) + 1))
		return (1);
	if (!ft_strncmp("pwd", *cmd, ft_strlen(*cmd) + 1))
		return (1);
	if (!ft_strncmp("env", *cmd, ft_strlen(*cmd) + 1))
		ft_env(mini);
	if (!ft_strncmp("unset", *cmd, ft_strlen(*cmd) + 1))
		ft_unset(cmd, mini);
	if (!ft_strncmp("cd", *cmd, ft_strlen(*cmd) + 1))
		ft_cd(cmd, mini);
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
		if (!is_a_builtin(current->args, mini, current->in_child))
		{
			// if (current->in_child)
			// {
			// 	//exec_w_child();
			// }
			// else
			exec_single(current->args, mini->env);
		}		
		if (current->next)
			current->next->in_child = 1;
		current = current->next;
	}
	return(1);
}