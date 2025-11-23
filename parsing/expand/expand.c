#include "../includes/minishell.h"

typedef struct s_expandinette
{
    int end;
    char *var_name;
    char *var_value;
    char char_str[2];
    char *result;
    char *temp;
    int i;    
}               t_expandinette;

typedef struct s_r_quote
{
    char *result;
    int i;
    int j;
    int len;
    t_quote status;
    char quote_char;
}               t_r_quote;

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
	while(j < len && is_char(str[beg_token]))
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
			env_value = ft_strdup_gc(minishell->env[i] + y, &minishell);
            free(token);
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

// char *do_expandV2(t_minishell *minishell, char *str)
// {
// 	char *ret;
// 	char ret_buf[BUFF_SIZE];
// 	int buf_pos = 0;
// 	int str_pos = 0;
// 	char *token;
// 	char *exp_ret;
// 	int i;
// 	int exp_len;
// 	t_quote status = NONE;

// 	while(str[str_pos])
// 	{
// 		set_quote_status(str[str_pos], &status);

// 		if(str[str_pos] == '$' &&  (status == NONE || status == DOUBLE)) 
// 		{ 
// 			token = get_expand_token(str, str_pos);
// 			exp_ret = do_expand(minishell, token);
// 			exp_len = ft_strlen(exp_ret);
// 			i = 0;
// 			while(i < exp_len)
// 			{
// 				ret_buf[buf_pos] = exp_ret[i];
// 				buf_pos++;
// 				i++;
// 			}
// 			str_pos += ft_strlen(token);
// 		}
// 		else
// 		{
// 			ret_buf[buf_pos] = str[str_pos];
// 			buf_pos++;
// 			str_pos++;
// 		}
// 	}
// 	ret = malloc(sizeof(char) * buf_pos + 1);
// 	if (!ret)
// 		return NULL;
// 	ft_memmove(ret, ret_buf, buf_pos);
// 	ret[buf_pos] = '\0';
// 	return (ret);
// }



void    set_quote(t_r_quote *r_quote, char *str)
{
    r_quote->i = 0;
    r_quote->j = 0;
    r_quote->len = ft_strlen(str);
    r_quote->status = NONE;
    r_quote->quote_char = '\0';
    r_quote->result = malloc(r_quote->len + 1);
 
}

char *remove_quotes(char *str)
{
    t_r_quote r_quote;
    
    set_quote(&r_quote, str);
    if (!r_quote.result)
        return (NULL);
    while (str[r_quote.i])
    {
        if (r_quote.status == NONE && (str[r_quote.i] == '\'' || str[r_quote.i] == '"'))
        {
            r_quote.quote_char = str[r_quote.i];
            r_quote.status = (str[r_quote.i] == '\'') ? SINGLE : DOUBLE;
        }
        else if (r_quote.status != NONE && str[r_quote.i] == r_quote.quote_char)
        {
            r_quote.status = NONE;
            r_quote.quote_char = '\0';
        }
        else
            r_quote.result[r_quote.j++] = str[r_quote.i];
        r_quote.i++;
    }
    if (str)
        free(str);
    r_quote.result[r_quote.j] = '\0';
    return (r_quote.result);
}

char *get_env_value(t_minishell *minishell, char *var_name)
{
    int i;
    int len;
    
    i = 0;
    len = ft_strlen(var_name);
    if (ft_strncmp(var_name, "?", 1) == 0 && ft_strlen(var_name) == 1)
        return (ft_itoa(minishell->status));
    if (ft_strncmp(var_name, "$", 1) == 0 && ft_strlen(var_name) == 1)
        return (ft_itoa(getpid()));
    while (minishell->env[i])
    {
        if (ft_strncmp(var_name, minishell->env[i], len) == 0 
            && minishell->env[i][len] == '=')
        {
            return (ft_strdup_gc(minishell->env[i] + len + 1, &minishell));
        }
        i++;
    }
    return (ft_strdup_gc("", &minishell));
}

char *extract_var_name(char *str, int start, int *end, t_minishell **mini)
{
    int i;
    
    i = start;
    if (str[i] == '?' || str[i] == '$')
    {
        *end = i + 1;
        return (ft_substr(str, i, 1));
    }
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
    *end = i;
    if (i == start)
        return (ft_strdup_gc("", mini));
    return (ft_substr(str, start, i - start));
}

int check_dolar_sign(char *str, int i)
{
    if (str[i] == '$' && str[i + 1] && str[i + 1] != '"')
        return(0);
    return (1);
}

void expandinette(t_minishell *minishell, t_expandinette *exp, char *str)
{
    exp->var_name = extract_var_name(str, exp->i + 1, &exp->end, &minishell);
    exp->var_value = get_env_value(minishell, exp->var_name);
    exp->temp = ft_strjoin(exp->result, exp->var_value);
    free(exp->result);
    exp->result = exp->temp;
    exp->i = exp->end;
    free(exp->var_name);
    free(exp->var_value);
}

void    set_expand(t_expandinette *exp, t_quote *status, t_minishell *mini)
{
    exp->i = 0;
    *status = NONE;
    exp->result = ft_strdup_gc("", &mini);
}

char *do_expand_simple(t_minishell *minishell, char *str)
{
    t_quote status;
    t_expandinette exp;
 
    set_expand(&exp, &status, minishell);
    if (!exp.result)
        return (NULL);
    while (str[exp.i])
    {
        set_quote_status(str[exp.i], &status);
        if (str[exp.i] == '$' && str[exp.i + 1] && str[exp.i + 1] != '"' 
                && str[exp.i + 1] != '\'' && status != SINGLE)
			expandinette(minishell, &exp, str);
        else
        {
            exp.char_str[0] = str[exp.i];
            exp.char_str[1] = '\0';
            exp.temp = ft_strjoin(exp.result, exp.char_str);
            exp.result = exp.temp;
            exp.i++;
        }
    }
    return (exp.result);
}
