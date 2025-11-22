/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emle-vou <emle-vou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 19:27:36 by emle-vou          #+#    #+#             */
/*   Updated: 2025/11/22 19:55:42 by emle-vou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_valuee(char **env, const char *name)
{
	int		i;
	size_t	len;

	if (!env || !name)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

static void	set_env_var(char **env, char *name, char *value)
{
	int		i;
	size_t	len;
	char	*new;
	char	*tmp;

	len = ft_strlen(name);
	new = ft_strjoin(name, "=");
	if (!new)
		return ;
	tmp = ft_strjoin(new, value);
	free(new);
	new = tmp;
	if (!new)
		return ;
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
		{
			free(env[i]);
			env[i] = new;
			return ;
		}
	}
	free(new);
}

static void	update_pwd_env(t_minishell *mini, char *old_pwd)
{
	char	*pwd;

	if (!mini || !mini->env)
		return ;
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return ;
	set_env_var(mini->env, "PWD", pwd);
	set_env_var(mini->env, "OLDPWD", old_pwd);
	free(pwd);
}

static int	get_cd_path(char **argv, t_minishell *mini, char **path)
{
	if (!argv[1] || argv[1][0] == '~')
	{
		*path = get_env_valuee(mini->env, "HOME");
		if (!*path)
			return (cd_error("HOME not set"));
	}
	else if (argv[1][0] == '-' && argv[1][1] == '\0')
	{
		*path = get_env_valuee(mini->env, "OLDPWD");
		if (!*path)
			return (cd_error("OLDPWD not set"));
		ft_putendl_fd(*path, 1);
	}
	else
		*path = argv[1];
	return (0);
}

int	ft_cd(char **argv, t_minishell *mini)
{
	char	*path;
	char	*old_pwd;

	if (!mini || !mini->env)
		return (1);
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (cd_error("error retrieving current directory"));
	if (get_cd_path(argv, mini, &path))
	{
		free(old_pwd);
		return (1);
	}
	if (chdir(path) != 0)
	{
		perror_cd(path);
		free(old_pwd);
		return (1);
	}
	update_pwd_env(mini, old_pwd);
	free(old_pwd);
	return (0);
}
