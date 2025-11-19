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
	perror("Error");
	exit(EXIT_FAILURE);
}

char *get_current_path()
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		return (ft_error("getcwd", strerror(errno)), NULL);
	return (path);
}

// void exec_mult(char **cmd, char **envp)
// {

// }

void	exec_single(char **cmd, char **envp)
{
	char	*path;
	int		pid;

	path = NULL;
	if (cmd[0][0] == '.' && cmd[0][1] == '/')
	{
		if (access(cmd[0], F_OK | X_OK) == 0)
			path = ft_strdup(cmd[0]);
		else
		{
			ft_putstr_fd("Error: command not found: ", 2);
			ft_putstr_fd(cmd[0], 2);
			ft_putstr_fd("\n", 2);
			return;
		}
		// if (execve(get_current_path(), cmd, envp) == -1) // le path de get_cur_path ne sera probablement jamais free sauf si execve le fait, je peux le load dans un str avant et free apres exec
		// 	error();
	}
	else 
	{
		if (access(cmd[0], F_OK | X_OK) == 0)
			path = find_path(cmd[0], envp);
		if (!path)	
		{
			ft_putstr_fd("Error: command not found: ", 2);
			ft_putstr_fd(cmd[0], 2);
			ft_putstr_fd("\n", 2);
			return;
		} // a replace par un printf_fd pour passer sous les 25 lignes
	}
	pid = fork();
	if (pid == 0)
	{
		if (execve(path, cmd, envp) == -1)
			error();
	}
	else if (pid > 0)
	{
		wait(NULL);
	}
	else
		error();
}


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
		// if (ft_strncmp("cd", cmd, ft_strlen(cmd) + 1))

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
		// if (!ft_strncmp("unset", *cmd, ft_strlen(*cmd) + 1))
		// 	ft_unset(cmd, mini);
		return(1);
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
		if (!is_a_builtin(current->args, mini, current->in_child))
		{
			// if (current->args[1])
			// 	//exec_mult(current->args, mini->env);
			// else
			exec_single(current->args, mini->env);
		}
		if (current->next)
			current->next->in_child = 1;
		current = current->next;
	}
	return(1);
}