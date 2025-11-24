/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 22:31:55 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 22:41:16 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_pid_one(t_cmd *cmd, t_minishell *mini, char *path)
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
	execve(path, cmd->args, mini->env);
	perror("execve");
	exit(127);
}

void	do_exe_sin(t_cmd *cmd, int pid, t_minishell *mini)
{
	int	status;

	if (cmd->fd_in != -1)
		close(cmd->fd_in);
	if (cmd->fd_out != -1)
		close(cmd->fd_out);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		mini->status = WEXITSTATUS(status);
}

void	exec_single(t_cmd *cmd, t_minishell *mini)
{
	char	*path;
	int		pid;

	if (!mini->env)
		return ;
	if (!check_exec(cmd, mini, &path))
	{
		mini->status = 127;
		return ;
	}
	pid = fork();
	if (pid == 0)
		do_pid_one(cmd, mini, path);
	else if (pid > 0)
		do_exe_sin(cmd, pid, mini);
	else
		error();
	free(path);
}

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*next;

	while (cmd)
	{
		next = cmd->next;
		free(cmd);
		cmd = next;
	}
}
