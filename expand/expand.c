#include "../includes/minishell.h"

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

int check_for_expand(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if (str[i] == '$')
            return(i + 1);
        i++;
    }
    return(0);
}


int	get_word_len(char *str)
{
	int len;

	len = 0;
	while(is_char(str[len]))
		len++;
	return(len);
}
char *get_expand_token(char *str)
{
	int i;
	char *res;

	i = 0;






	while (str[i])
	{
		if (str[i] == '$')
		{
			res = malloc(sizeof(char) * get_word_len(str + i));
			if (!res)
				return(NULL);
			i++;
			while(!is_sep(str[i]))
			{
				res[i] = str[i];
				i++;
			}
		}
		i++;
	}
	return (res);
}

char *do_expand(t_minishell *minishell, char *str)
{
	char *token;
	char *env_value;
	int i;
	int y;
	int expand_pos;

	if ((expand_pos = check_for_expand(str)) > 0)
		token = get_expand_token(str + expand_pos);
	else
		return(str);
	i = 0;
	while(minishell->env[i])
	{
		if (ft_strncmp(token, minishell->env[i], ft_strlen(token)) == 0 
			&& minishell->env[i][ft_strlen(token)] == '=')
		{
			y = ft_strlen(token) + 1;
			env_value = ft_strdup(minishell->env[i] + y);
            free(token);
            return (env_value);
        }
        i++;
    }
    free(token);
    return(str);
}
