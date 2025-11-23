/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emle-vou <emle-vou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 21:47:12 by emle-vou          #+#    #+#             */
/*   Updated: 2025/11/23 21:56:44 by emle-vou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (!path)
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
	check_exec(cmd, mini, &path);
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
	free(path);
}

int	execute(t_cmd *cmd, t_minishell *mini)
{
	if (!cmd)
		return (0);
	if (cmd->next)
		exec_mult(cmd, mini);
	else
	{
		if (!is_a_builtin(cmd->args, mini))
			exec_single(cmd, mini);
	}
	return (1);
}
