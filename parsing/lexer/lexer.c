#include "minishell.h"

/* I know a lexer is a tokenizer, go touch grass */

typedef struct s_lexer
{
	char	**cmd_tab;
	char	*heredoc;
	int		tab_idx;
	int		fd_in;
	int		fd_out;
	int		app_mode;
	int		words;
	int		ret_val;
}				t_lexer;

// strat faire une petite cmd qui set let var en bas et should be good
void	cmd_set(t_cmd *new, int fd_in, int fd_out, int app_mode, int i)
{
	new->args[i] = NULL;
	new->fd_in = fd_in;
	new->fd_out = fd_out;
	new->append_mode = app_mode;
	new->next = NULL;
	new->heredoc = NULL;
	new->in_child = 0;
}

char	*ft_strdup_gc(char *src, t_minishell *mini)
{
	int		i;
	char	*dup;

	i = 0;
	dup = (char *)ft_malloc((ft_strlen(src) + 1) * sizeof(char), &mini->alloc);
	if (!dup)
		return (NULL);
	while (src[i])
	{
		dup[i] = src[i];
		++i;
	}
	dup[i] = '\0';
	return (dup);
}

void	*ft_calloc_gc(size_t nbr_elements, size_t element_size, t_minishell *mini)
{
	void	*ptr;

	ptr = ft_malloc(nbr_elements * element_size, &mini->alloc);
	if (ptr == NULL)
		return (NULL);
	ft_bzero(ptr, nbr_elements * element_size);
	return (ptr);
}

char	*ft_strjoin_gc(const char *s1, const char *s2, t_minishell *mini)
{
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	str = (char *)ft_calloc_gc((ft_strlen(s1) + ft_strlen(s2)) + 1, sizeof(char), &mini->alloc);
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, ft_strlen(s1) + 1);
	ft_strlcat(str, s2, ft_strlen(s1) + ft_strlen(s2) + 1);
	return (str);
}


t_cmd	*cmd_create(char **cmds, int fd_in, int fd_out, int app_mode)
{
	t_cmd	*new;
	int		i;
	int		count;

	if (!cmds)
		return (NULL);
	count = 0;
	while (cmds[count])
		count++;
	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->args = ft_calloc(count + 1, sizeof(char *));
	if (!new->args)
	{
		free(new);
		return (NULL);
	}
	i = -1;
	while (++i < count)
		new->args[i] = ft_strdup(cmds[i]);
	cmd_set(new, fd_in, fd_out, app_mode, i);
	free(cmds);
	return (new);
}

void	cmd_append(t_cmd **cmd, t_cmd *new_node)
{
	t_cmd	*current;

	if (!*cmd)
	{
		*cmd = new_node;
		return ;
	}
	current = *cmd;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

void	print_lexer(t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		print_env(current->args);
		printf("%i\n", current->fd_in);
		printf("%i\n", current->fd_out);
		printf("%i\n", current->append_mode);
		if (current->heredoc)
			printf("%s\n", current->heredoc);
		current = current->next;
	}
}

/*
	strat,
		le dernier else peut pot etre une fonc mais ca va etre chaud pour le reste
*/
int	be_a_heredoc(char **line, char **ret_line, char **limiter, char **line_nl)
{
	char	*tmp;

	if (ft_strncmp(*line, *limiter, ft_strlen(*limiter)) == 0)
	{
		free(*line);
		return (0);
	}
	*line_nl = ft_strjoin(*line, "\n");
	free(*line);
	if (!*line_nl)
		return (free(ret_line), -1);
	if (!*ret_line)
		*ret_line = *line_nl;
	else
	{
		tmp = *ret_line;
		*ret_line = ft_strjoin(*ret_line, *line_nl);
		free(tmp);
		free(*line_nl);
		if (!*ret_line)
			return (-1);
	}
	return (1);
}

typedef struct s_heredoc
{
	char	*line;
    char	*ret_line;
    char	*line_nl;
    int		here_ret;
}				t_heredoc;

int	do_heredoc(char *limiter, char **out)
{
    t_heredoc her;

    if (!limiter || !out)
        return (-1);
    if (*out)
    {
        free(*out);
        *out = NULL;
    }
    her.ret_line = NULL;
    while ((her.line = readline("Theredoc>")) != NULL)
    {
        her.here_ret = be_a_heredoc(&her.line, &her.ret_line, &limiter, &her.line_nl);
        if (her.here_ret == 0)
            break;
        else if (her.here_ret == -1)
            return (-1);
    }
    if (her.line == NULL)
        printf("minishell: warning: here-document\n");
    if (!her.ret_line)
        her.ret_line = ft_strdup("");
    *out = her.ret_line; 
    return (0);
}

static int	count_words_until_pipe(t_token *start)
{
	int	count;

	count = 0;
	while (start && start->type != PIPE)
	{
		if (start->type == WORD)
			count++;
		start = start->next;
	}
	return (count);
}

int	do_type_word(t_lexer *lexer, t_token **current)
{
	if (!lexer->cmd_tab)
	{
		lexer->words = count_words_until_pipe(*current);
		lexer->cmd_tab = ft_calloc(lexer->words + 1, sizeof(char *));
		if (!lexer->cmd_tab)
			return (0);
		lexer->tab_idx = 0;
	}
	lexer->cmd_tab[lexer->tab_idx++] = ft_strdup((*current)->value);
	*current = (*current)->next;
	return (1);
}

int	do_type_pipe(t_token **current, t_cmd **new_node, t_lexer *lexer, t_cmd **cmd)
{
	(*new_node) = cmd_create(lexer->cmd_tab, lexer->fd_in, lexer->fd_out, lexer->app_mode);
	if (!(*new_node))
		return (0);
	(*new_node)->heredoc = lexer->heredoc;
	lexer->heredoc = NULL;
	cmd_append(cmd, (*new_node));
	lexer->cmd_tab = NULL;
	lexer->tab_idx = 0;
	lexer->fd_in = -1;
	lexer->fd_out = -1;
	lexer->app_mode = 0;
	(*current) = (*current)->next;
	return (1);
}

int do_type_redirapp(t_lexer *lexer, t_token **current)
{
	lexer->fd_out = open((*current)->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (lexer->fd_out == -1)
		return (0);
	*current = (*current)->next->next;
	return (1);
}

int do_type_redirher(t_lexer *lexer, t_token **current)
{
	if (lexer->heredoc)
    {
        free(lexer->heredoc);
        lexer->heredoc = NULL;
    }
	if (do_heredoc((*current)->next->value, &lexer->heredoc) == -1)
		return (0);
	*current = (*current)->next->next;
	return (1);
}



int do_type_redirin(t_lexer *lexer, t_token **current)
{
	lexer->fd_in = open((*current)->next->value, O_RDONLY);
	if (lexer->fd_in == -1)
		return (0);
	*current = (*current)->next->next;
	return (1);
}


int do_type_redirout(t_lexer *lexer, t_token **current)
{
	lexer->fd_out = open((*current)->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);  // est-ce qu'on create si il n'existe pas ?
	if (lexer->fd_out == -1)
		return (0);
	*current = (*current)->next->next;
	return (1);
}

void set_lexer(t_lexer **lexer)
{
	*lexer = malloc(sizeof(t_lexer));
	(*lexer)->cmd_tab = NULL;
	(*lexer)->tab_idx = 0;
	(*lexer)->app_mode = 0;
	(*lexer)->fd_in = -1;
	(*lexer)->fd_out = -1;
	(*lexer)->ret_val = 1;
	(*lexer)->heredoc = NULL;
}

int do_lexing(t_lexer *lexer, t_cmd **new_node, t_cmd **cmd)
{
	(*new_node) = cmd_create(lexer->cmd_tab, lexer->fd_in, lexer->fd_out, lexer->app_mode);
	if (!*new_node)
		return (0);
	(*new_node)->heredoc = lexer->heredoc;
	cmd_append(cmd, *new_node);
	return(1);
}

/*
	strat,
		faire une struct avec toutes les var et faire des micro fonc pour chaque if de type qui prend juste la struct en param
*/
int lexinette(t_lexer *lexer, t_token **current, t_cmd **new_node, t_cmd **cmd)
{
	if ((*current)->type == WORD)
		lexer->ret_val = do_type_word(lexer, current);
	else if (!(*current)->next)
		return (0);
	else if ((*current)->type == REDIRECT_APPEND)
		lexer->ret_val = do_type_redirapp(lexer, current);
	else if ((*current)->type == REDIRECT_HEREDOC)
		lexer->ret_val = do_type_redirher(lexer, current);
	else if ((*current)->type == REDIRECT_IN)
		lexer->ret_val = do_type_redirin(lexer, current);
	else if ((*current)->type == REDIRECT_OUT)
		lexer->ret_val = do_type_redirout(lexer, current);
	else if ((*current)->type == PIPE)
		lexer->ret_val = do_type_pipe(current, new_node, lexer, cmd);
	else
		*current = (*current)->next;
	return (lexer->ret_val);
}

t_cmd *lexer(t_token *token, t_cmd *cmd)
{
	t_token	*current;
	t_lexer *lexer;
	t_cmd	*new_node;

	current = token;
	set_lexer(&lexer);
	while (current)
		if (!lexinette(lexer, &current, &new_node, &cmd))
			return NULL;
	if (lexer->cmd_tab)
		do_lexing(lexer, &new_node, &cmd);
	return (cmd);
}
