/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 22:04:47 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 22:50:20 by baecoliv         ###   ########.fr       */
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

int	is_a_builtin(t_cmd *cmd, t_minishell *mini)
{
	if (check_for_builtin(cmd->args))
	{
		if (!ft_strncmp("cd", cmd->args[0], ft_strlen(cmd->args[0]) + 1))
			ft_cd(cmd->args, mini);
		if (!ft_strncmp("echo", cmd->args[0], 4)
			&& ft_strlen(cmd->args[0]) == 4)
			ft_echo(cmd);
		if (!ft_strncmp("exit", cmd->args[0], ft_strlen(cmd->args[0]) + 1))
			ft_exit(cmd->args, mini);
		if (!ft_strncmp("export", cmd->args[0], ft_strlen(cmd->args[0]) + 1))
			ft_export(mini, cmd->args);
		if (!ft_strncmp("pwd", cmd->args[0], ft_strlen(cmd->args[0]) + 1))
			ft_pwd();
		if (!ft_strncmp("env", cmd->args[0], ft_strlen(cmd->args[0]) + 1))
			ft_env(mini);
		if (!ft_strncmp("unset", cmd->args[0], ft_strlen(cmd->args[0]) + 1))
			ft_unset(mini, cmd->args);
		return (1);
	}
	return (0);
}

void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
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
			return (free_paths(paths), path);
		free(path);
		i++;
	}
	i = -1;
	free_paths(paths);
	return (0);
}

void	error(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}
