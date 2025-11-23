/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emle-vou <emle-vou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 21:52:43 by emle-vou          #+#    #+#             */
/*   Updated: 2025/11/23 22:28:03 by emle-vou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
