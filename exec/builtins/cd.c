// #include "../../includes/minishell.h"

// static char	*get_env_value(char **env, const char *name)
// {
// 	int		i;
// 	size_t	len;

// 	if (!env || !name)
// 		return (NULL);
// 	len = ft_strlen(name);
// 	i = 0;
// 	while (env[i])
// 	{
// 		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
// 			return (env[i] + len + 1);
// 		i++;
// 	}
// 	return (NULL);
// }

// static void	update_pwd_env(t_minishell *mini, char *old_pwd)
// {
// 	char	*pwd;
// 	char	*new_pwd;
// 	char	*new_oldpwd;
// 	int		i;

// 	pwd = getcwd(NULL, 0);
// 	if (!pwd)
// 		return ;
	
// 	new_pwd = ft_strjoin("PWD=", pwd);
// 	new_oldpwd = ft_strjoin("OLDPWD=", old_pwd);
// 	if (!new_pwd || !new_oldpwd)
// 	{
// 		free(pwd);
// 		free(new_pwd);
// 		free(new_oldpwd);
// 		return ;
// 	}
	
// 	i = 0;
// 	while (mini->env[i])
// 	{
// 		if (ft_strncmp(mini->env[i], "PWD=", 4) == 0)
// 		{
// 			free(mini->env[i]);
// 			mini->env[i] = new_pwd;
// 		}
// 		else if (ft_strncmp(mini->env[i], "OLDPWD=", 7) == 0)
// 		{
// 			free(mini->env[i]);
// 			mini->env[i] = new_oldpwd;
// 		}
// 		i++;
// 	}
// 	free(pwd);
// }

// int	ft_cd(char **argv, t_minishell *mini)
// {
// 	char	*path;
// 	char	*old_pwd;

// 	if (!mini || !mini->env)
// 		return (1);
	
// 	old_pwd = getcwd(NULL, 0);
// 	if (!old_pwd)
// 	{
// 		ft_putstr_fd("minishell: cd: error retrieving current directory\n", 2);
// 		return (1);
// 	}

// 	if (!argv[1] || ft_strcmp(argv[1], "~") == 0)
// 	{
// 		path = get_env_value(mini->env, "HOME");
// 		if (!path)
// 		{
// 			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
// 			free(old_pwd);
// 			return (1);
// 		}
// 	}
// 	else if (ft_strcmp(argv[1], "-") == 0)
// 	{
// 		path = get_env_value(mini->env, "OLDPWD");
// 		if (!path)
// 		{
// 			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
// 			free(old_pwd);
// 			return (1);
// 		}
// 		ft_putendl_fd(path, 1);
// 	}
// 	else
// 		path = argv[1];

// 	if (chdir(path) != 0)
// 	{
// 		ft_putstr_fd("minishell: cd: ", 2);
// 		ft_putstr_fd(path, 2);
// 		ft_putstr_fd(": ", 2);
// 		ft_putstr_fd(strerror(errno), 2);
// 		ft_putstr_fd("\n", 2);
// 		free(old_pwd);
// 		return (1);
// 	}

// 	update_pwd_env(mini, old_pwd);
// 	free(old_pwd);
// 	return (0);
// }