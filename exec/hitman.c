#include "minishell.h"

typedef struct s_exec
{
	int		fd[2];
	int		prev_fd;
	int		pid;
	char	*path;
}			t_exec;

int	check_for_builtin(char **cmd)
{
	if (!cmd || !*cmd)
		return (0);
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
	if (!ft_strncmp("cd", *cmd, ft_strlen(*cmd) + 1))
		return (1);
	if (!ft_strncmp("unset", *cmd, ft_strlen(*cmd) + 1))
		return (1);
	return (0);
}

int	is_a_builtin(char **cmd, t_minishell *mini)
{
	if (check_for_builtin(cmd))
	{
		if (!ft_strncmp("cd", *cmd, ft_strlen(*cmd) + 1))
			ft_cd(cmd, mini);
		if (!ft_strncmp("echo", cmd[0], 4) && ft_strlen(cmd[0]) == 4)
			ft_echo(*mini, cmd);
		if (!ft_strncmp("exit", *cmd, ft_strlen(*cmd) + 1))
			ft_exit(cmd, mini);
		if (!ft_strncmp("export", *cmd, ft_strlen(*cmd) + 1))
			ft_export(mini, cmd);
		if (!ft_strncmp("pwd", *cmd, ft_strlen(*cmd) + 1))
			ft_pwd();
		if (!ft_strncmp("env", *cmd, ft_strlen(*cmd) + 1))
			ft_env(mini);
		if (!ft_strncmp("unset", *cmd, ft_strlen(*cmd) + 1))
			ft_unset(mini, cmd);
		return (1);
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
	if (!cmd || !envp)
		return (NULL);
	while (envp[i] && ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
		{
			i = -1;
			while (paths[++i])
				free(paths[i]);
			free(paths);
			return (path);
		}
		free(path);
		i++;
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (NULL);
}

void	error(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}

char	*get_current_path(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		return (ft_error("getcwd", strerror(errno)), NULL);
	return (path);
}

static void	put_err_msg(char *str)
{
	ft_putstr_fd("Error: command not found: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
}

void	handle_heredoc(t_cmd *current)
{
	int	pipe_heredoc[2];

	if (pipe(pipe_heredoc) == -1)
		error();
	write(pipe_heredoc[1], current->heredoc, ft_strlen(current->heredoc));
	close(pipe_heredoc[1]);
	dup2(pipe_heredoc[0], STDIN_FILENO);
	close(pipe_heredoc[0]);
}

void	dup_close(int fd_dup, int std, int fd_close)
{
	dup2(fd_dup, std);
	close(fd_close);
}

void	handle_pid_one_bis(t_cmd *current, t_minishell *mini, char *path)
{
	if (current->heredoc)
		handle_heredoc(current);
	if (is_a_builtin(current->args, mini))
		exit(0);
	if (mini->env)
		path = find_path(current->args[0], mini->env);
	if (!path)
	{
		put_err_msg(current->args[0]);
		exit(127);
	}
	if (execve(path, current->args, mini->env) == -1)
	{
		free(path);
		perror("execve");
		exit(1);
	}
}

void	handle_pid_one(t_cmd *current, t_exec exec, t_minishell *mini,
		char *path)
{
	if (current->fd_in != -1)
		dup_close(current->fd_in, STDIN_FILENO, current->fd_in);
	else if (exec.prev_fd != -1)
		dup_close(exec.prev_fd, STDIN_FILENO, exec.prev_fd);
	if (current->fd_out != -1)
		dup_close(current->fd_out, STDOUT_FILENO, current->fd_out);
	else if (current->next)
	{
		dup2(exec.fd[1], STDOUT_FILENO);
		close(exec.fd[1]);
		close(exec.fd[0]);
	}
	handle_pid_one_bis(current, mini, path);
}

void	handle_parent(t_cmd *current, t_exec *exec)
{
	if (current->fd_in != -1)
		close(current->fd_in);
	if (current->fd_out != -1)
		close(current->fd_out);
	if (exec->prev_fd != -1)
		close(exec->prev_fd);
	if (current->next)
	{
		close(exec->fd[1]);
		exec->prev_fd = exec->fd[0];
	}
}

void	exec_mult(t_cmd *cmd, t_minishell *mini)
{
	t_exec	exec;
	t_cmd	*current;

	exec.prev_fd = -1;
	current = cmd;
	while (current)
	{
		if (current->next && pipe(exec.fd) == -1)
			error();
		exec.pid = fork();
		if (exec.pid == 0)
			handle_pid_one(current, exec, mini, exec.path);
		else if (exec.pid > 0)
			handle_parent(current, &exec);
		else
			error();
		current = current->next;
	}
	if (exec.prev_fd != -1)
		close(exec.prev_fd);
	while (wait(NULL) > 0)
		;
}

int	check_exec(t_cmd *cmd, t_minishell *mini, char **path)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (cmd->args[0][0] == '.' && cmd->args[0][1] == '/')
	{
		if (access(cmd->args[0], F_OK | X_OK) == 0)
			*path = ft_strdup(cmd->args[0]);
		else
		{
			put_err_msg(cmd->args[0]);
			return (0);
		}
	}
	else
	{
		*path = find_path(cmd->args[0], mini->env);
		if (!*path)
		{
			put_err_msg(cmd->args[0]);
			return (0);
		}
	}
	return (1);
}

void	do_pid_one(t_cmd *cmd, t_minishell *mini, char *path)
{
	int	pipe_heredoc[2];

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

void	exec_single(t_cmd *cmd, t_minishell *mini)
{
	char	*path;
	int		pid;

	if (!mini->env)
		return ;
	path = NULL;
	if (!check_exec(cmd, mini, &path))
		return ;
	pid = fork();
	if (pid == 0)
		do_pid_one(cmd, mini, path);
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
	if (path)
		free(path);
}

int	execute(t_cmd *cmd, t_minishell *mini)
{
	int	saved_stdin;
	int	saved_stdout;

	if (!cmd)
		return (0);
	if (cmd->next)
		exec_mult(cmd, mini);
	else
	{
		if (check_for_builtin(cmd->args))
		{
			saved_stdin = -1;
			saved_stdout = -1;
			if (cmd->fd_in != -1)
			{
				saved_stdin = dup(STDIN_FILENO);
				dup2(cmd->fd_in, STDIN_FILENO);
				close(cmd->fd_in);
			}
			if (cmd->fd_out != -1)
			{
				saved_stdout = dup(STDOUT_FILENO);
				dup2(cmd->fd_out, STDOUT_FILENO);
				close(cmd->fd_out);
			}
			is_a_builtin(cmd->args, mini);
			if (saved_stdin != -1)
			{
				dup2(saved_stdin, STDIN_FILENO);
				close(saved_stdin);
			}
			if (saved_stdout != -1)
			{
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdout);
			}
		}
		else
			exec_single(cmd, mini);
	}
	return (1);
}