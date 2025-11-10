#include "../../includes/minishell.h"

/*
** unset command removes environment variables from the shell environment
** Returns 0 on success, 1 if invalid arguments are provided
*/

static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static void	remove_env_var(char ***env, const char *name)
{
	int		i;
	int		j;
	char	**new_env;
	size_t	name_len;

	if (!env || !*env || !name)
		return ;
	i = 0;
	while ((*env)[i])
		i++;
	new_env = (char **)malloc(sizeof(char *) * i);
	if (!new_env)
		return ;
	i = 0;
	j = 0;
	name_len = ft_strlen(name);
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], name, name_len) != 0 || 
			(*env)[i][name_len] != '=')
			new_env[j++] = ft_strdup((*env)[i]);
		i++;
	}
	new_env[j] = NULL;
	free_array(*env);
	*env = new_env;
}

int	ft_unset(char **argv, t_minishell *mini)
{
	int	i;
	int	status;

	status = 0;
	if (!argv || !argv[1] || !mini)
		return (0);
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else
			remove_env_var(&mini->env, argv[i]);
		i++;
	}
	return (status);
}