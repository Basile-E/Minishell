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
	while(str[len] && is_char(str[len]))
		len++;
	return(len);
}

char *get_expand_token(char *str, int beg_token)
{
	int j;
	char *res;
	int len;

	beg_token++;
	len = get_word_len(str + beg_token);
	res = malloc(sizeof(char) * (len + 1));
	if (!res)
		return(NULL);
	j = 0;
	while(j < len && is_char(str[beg_token])) //!is_sep(str[beg_token])
	{
		res[j] = str[beg_token];
		beg_token++;
		j++;
	}
	res[j] = '\0';
	return (res);
}

char *do_expand(t_minishell *minishell, char *token)
{
	char *env_value;
	int i;
	int y;

	i = 0;
	while(minishell->env[i])
	{
		if (ft_strncmp(token, minishell->env[i], ft_strlen(token)) == 0 
			&& minishell->env[i][ft_strlen(token)] == '=')
		{
			y = ft_strlen(token) + 1;
			env_value = ft_strdup(minishell->env[i] + y);
            free(token); // pas sur, token atais initialiser ici mais le code a changer depuis;
            return (env_value);
        }
        i++;
    }
    return(token);
}

char *do_expandV2(t_minishell *minishell, char *str)
{
	char *ret;
	char ret_buf[BUFF_SIZE];
	int buf_pos = 0;
	int str_pos = 0;
	char *token;
	char *exp_ret;
	int i;
	int exp_len;
	t_quote status;

	(void) status;
	while(str[str_pos])
	{
		if(str[str_pos] == '$')
		{
			token = get_expand_token(str, str_pos);
			exp_ret = do_expand(minishell, token);
			exp_len = ft_strlen(exp_ret);
			i = 0;
			while(i < exp_len)
			{
				ret_buf[buf_pos] = exp_ret[i];
				buf_pos++;
				i++;
			}
			str_pos += ft_strlen(token) + 1;
		}
		else
		{
			ret_buf[buf_pos] = str[str_pos];
			buf_pos++;
			str_pos++;
		}
	}
	ret = malloc(sizeof(char) * buf_pos + 1);
	if (!ret)
		return NULL;
	ft_memmove(ret, ret_buf, buf_pos);
	ret[buf_pos] = '\0';
	return (ret);
}



/*

	str = Bonjour$USER$test1$'test2'"$test3"



*/