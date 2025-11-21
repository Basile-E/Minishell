#include "minishell.h"

/*
	pseudo exec pipes :
		il y a un mega monde où je peux faire une fonction qui reprend l'exec de pipex
		et fait une boucle ou on ouvre un fd on envoie on exec avec le fd et on le ferme
		puis on ouvre un fd sur l'output de la cmd et rebelotte tant qu'il y a des cmd[i + 1]

*/


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

void exec_mult(t_cmd *cmd, t_minishell *mini)
{
    int fd[2];
    int prev_fd = -1;
    int pid;
    char *path;
    t_cmd *current;

    current = cmd;
    while (current)
    {
        if (current->next && pipe(fd) == -1)
            error();
        pid = fork();
        if (pid == 0)
        {
            // Handle input redirection
            if (current->fd_in != -1)
            {
                dup2(current->fd_in, STDIN_FILENO);
                close(current->fd_in);
            }
            else if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            
            // Handle output redirection
            if (current->fd_out != -1)
            {
                dup2(current->fd_out, STDOUT_FILENO);
                close(current->fd_out);
            }
            else if (current->next)
            {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
                close(fd[0]);
            }
            
            // Handle heredoc
            if (current->heredoc)
            {
                int pipe_heredoc[2];
                if (pipe(pipe_heredoc) == -1)
                    error();
                write(pipe_heredoc[1], current->heredoc, ft_strlen(current->heredoc));
                close(pipe_heredoc[1]);
                dup2(pipe_heredoc[0], STDIN_FILENO);
                close(pipe_heredoc[0]);
            }
            
            if (is_a_builtin(current->args, mini, 1))
                exit(0);
            if (mini->env)
                path = find_path(current->args[0], mini->env);
            if (!path)
            {
                ft_putstr_fd("Command not found: ", 2);
                ft_putstr_fd(current->args[0], 2);
                ft_putstr_fd("\n", 2);
                exit(127);
            }
            if (execve(path, current->args, mini->env) == -1)
            {
                free(path);
                perror("execve");
                exit(1);
            }
        }
        else if (pid > 0)
        {
            // Close file descriptors in parent
            if (current->fd_in != -1)
                close(current->fd_in);
            if (current->fd_out != -1)
                close(current->fd_out);
                
            if (prev_fd != -1)
                close(prev_fd);
            if (current->next)
            {
                close(fd[1]);
                prev_fd = fd[0];
            }
        }
        else
            error();
        current = current->next;
    }
    if (prev_fd != -1)
        close(prev_fd);
    while (wait(NULL) > 0)
        ;
}


void	exec_single(t_cmd *cmd, t_minishell *mini)
{
    char *path;
    int pid;

    if (cmd->args[0][0] == '.' && cmd->args[0][1] == '/')
    {
        if (access(cmd->args[0], F_OK | X_OK) == 0)
            path = ft_strdup(cmd->args[0]);
        else
        {
            ft_putstr_fd("Error: command not found: ", 2);
            ft_putstr_fd(cmd->args[0], 2);
            ft_putstr_fd("\n", 2);
            return;
        }
    }
    else 
    {
        if (!mini->env)
            return;
        path = find_path(cmd->args[0], mini->env);
        if (!path)    
        {
            ft_putstr_fd("Error: command not found: ", 2);
            ft_putstr_fd(cmd->args[0], 2);
            ft_putstr_fd("\n", 2);
            return;
        }
    }
    
    pid = fork();
    if (pid == 0)
    {
        if (cmd->fd_in != -1)
        {
            dup2(cmd->fd_in, STDIN_FILENO);
            close(cmd->fd_in);
        }
        if (cmd->fd_out != -1)
        {
            dup2(cmd->fd_out, STDOUT_FILENO);
            close(cmd->fd_out);
        }
        if (cmd->heredoc)
        {
            int pipe_heredoc[2];
            if (pipe(pipe_heredoc) == -1)
                error();
            write(pipe_heredoc[1], cmd->heredoc, ft_strlen(cmd->heredoc));
            close(pipe_heredoc[1]);
            dup2(pipe_heredoc[0], STDIN_FILENO);
            close(pipe_heredoc[0]);
        }
        if (execve(path, cmd->args, mini->env) == -1)
            error();
    }
    else if (pid > 0)
    {
        if (cmd->fd_in != -1)
            close(cmd->fd_in);
        if (cmd->fd_out != -1)
            close(cmd->fd_out);
        wait(NULL);
    }
    else
        error();
    
    free(path);
}

int execute(t_cmd *cmd, t_minishell *mini)
{
    if (!cmd)
        return 0;
    if (cmd->next)
        exec_mult(cmd, mini);
    else
    {
        if (!is_a_builtin(cmd->args, mini, 0))
            exec_single(cmd, mini);
    }
    return 1;
}
