/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 22:35:46 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 22:35:51 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_current_path(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		return (ft_error("getcwd", strerror(errno)), NULL);
	return (path);
}

void	put_err_msg(char *str)
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
	if (is_a_builtin(current, mini))
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
