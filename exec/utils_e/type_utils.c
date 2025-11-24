/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 22:13:18 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 22:13:19 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	exec_mult(t_cmd *current, t_minishell *mini)
{
	t_exec	exec;
	t_cmd	*cmd;

	exec.prev_fd = -1;
	cmd = current;
	while (cmd)
	{
		if (cmd->next && pipe(exec.fd) == -1)
			error();
		exec.pid = fork();
		if (exec.pid == 0)
			handle_pid_one(cmd, exec, mini, exec.path);
		else if (exec.pid > 0)
			handle_parent(cmd, &exec);
		else
			error();
		cmd = cmd->next;
	}
	if (exec.prev_fd != -1)
		close(exec.prev_fd);
	while (waitpid(-1, &mini->status, 0) > 0)
	{
		if (WIFEXITED(mini->status))
			mini->status = WEXITSTATUS(mini->status);
	}
}

int	check_exec(t_cmd *cmd, t_minishell *mini, char **path)
{
	if (cmd->args[0][0] == '.' && cmd->args[0][1] == '/')
	{
		if (access(cmd->args[0], F_OK | X_OK) == 0)
			*path = ft_strdup_gc(cmd->args[0], mini);
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
