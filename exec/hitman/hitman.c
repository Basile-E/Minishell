/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hitman.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emle-vou <emle-vou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 21:46:23 by emle-vou          #+#    #+#             */
/*   Updated: 2025/11/23 22:07:13 by emle-vou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_for_builtin(char **cmd)
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
