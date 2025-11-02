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

void	set_quote_status(char c, t_quote *status)
{
    if (*status == NONE && c == '\'')
        *status = SINGLE;
    else if (*status == NONE && c == '"')
        *status = DOUBLE;
    else if ((*status == SINGLE && c == '\'') ||
            (*status == DOUBLE && c == '"'))
        *status = NONE;
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
	t_quote status = NONE;

	while(str[str_pos])
	{
		set_quote_status(str[str_pos], &status);

		if(str[str_pos] == '$' &&  (status == NONE || status == DOUBLE)) 
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
			str_pos += ft_strlen(token);
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

// Fonction pour nettoyer les quotes d'une string
char *remove_quotes(char *str)
{
    char *result;
    int i = 0; 
	int j = 0;
    int len = ft_strlen(str);
    t_quote status = NONE;
    
    result = malloc(len + 1);
    if (!result)
        return (NULL);
    
    while (str[i])
    {
        if (str[i] == '\'' || str[i] == '"')
            set_quote_status(str[i], &status);
        else
        {
            result[j] = str[i];
            j++;
        }
        i++;
    }
	if (str)
		free(str);
    result[j] = '\0';
    return (result);
}

// Fonction pour obtenir une variable d'environnement
char *get_env_value(t_minishell *minishell, char *var_name)
{
    int i = 0;
    int len = ft_strlen(var_name);
    
    // Variables spéciales - REPLACE ft_strcmp with ft_strncmp
    if (ft_strncmp(var_name, "?", 1) == 0 && ft_strlen(var_name) == 1)
        return (ft_itoa(minishell->status));
    if (ft_strncmp(var_name, "$", 1) == 0 && ft_strlen(var_name) == 1)
        return (ft_itoa(getpid()));
    
    // Variables d'environnement normales
    while (minishell->env[i])
    {
        if (ft_strncmp(var_name, minishell->env[i], len) == 0 
            && minishell->env[i][len] == '=')
        {
            return (ft_strdup(minishell->env[i] + len + 1));
        }
        i++;
    }
    
    // Variable non trouvée = chaîne vide
    return (ft_strdup(""));
}

// Fonction pour extraire le nom de variable après $
char *extract_var_name(char *str, int start, int *end)
{
    int i = start;
    
    // Cas spéciaux : $? et $$
    if (str[i] == '?' || str[i] == '$')
    {
        *end = i + 1;
        return (ft_substr(str, i, 1));
    }
    
    // Variables normales : lettres, chiffres et _
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
    
    *end = i;
    if (i == start)
        return (ft_strdup(""));
    
    return (ft_substr(str, start, i - start));
}

// Fonction d'expansion simplifiée et corrigée
char *do_expand_simple(t_minishell *minishell, char *str)
{
    char *result;
    char *temp;
    int i = 0;
    t_quote status = NONE;

    
    result = ft_strdup(""); // Start with empty string
    if (!result)
        return (NULL);
    
    while (str[i])
    {
        set_quote_status(str[i], &status);
        
        if (str[i] == '$' && status != SINGLE)
        {
			int end;
            char *var_name = extract_var_name(str, i + 1, &end);
            char *var_value = get_env_value(minishell, var_name);
        
            // Concaténer la valeur (même si c'est une chaîne vide)
            temp = ft_strjoin(result, var_value);
            free(result);
            result = temp;
            i = end;
        
            free(var_name);
            free(var_value);	
        }
        else
        {
            char char_str[2];
            char_str[0] = str[i];
            char_str[1] = '\0';
            
            temp = ft_strjoin(result, char_str);
            free(result);
            result = temp;
            i++;
        }
    }
    
    return (result);
}

/*

	str = Bonjour$USER$test1$'test2'"$test3"



*/