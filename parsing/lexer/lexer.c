#include "minishell.h"

/* I know a lexer is a parser, go touch grass */

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
	{
		// caller still owns cmds
		return (NULL);
	}
	new->args = ft_calloc(count + 1, sizeof(char *));
	if (!new->args)
	{
		free(new);
		return (NULL);
	}
	i = 0;
	while (i < count)
	{
		new->args[i] = ft_strdup(cmds[i]);
		i++;
	}
	new->args[i] = NULL;
	new->fd_in = fd_in;
	new->fd_out = fd_out;
	new->append_mode = app_mode;
	new->next = NULL;
	new->heredoc = NULL;
	// caller passed cmds buffer ownership to us, free it
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

// est-ce qu'il faut absolument le charger dans un fd ? si oui est-ce que je dois le mettre dans mon fd in ou c'est un fd heredoc
int	do_heredoc(const char *limiter, char **out)
{
	char	*line;
	char	*ret_line;
	char	*line_nl;
	char	*tmp;

	if (!limiter || !out)
		return (-1);
	*out = NULL;
	ret_line = NULL;
	while ((line = readline("Theirdoc>")) != NULL)
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
		{
			free(line);
			break ;
		}
		line_nl = ft_strjoin(line, "\n");
		free(line);
		if (!line_nl)
		{
			free(ret_line);
			return (-1);
		}
		if (!ret_line)
			ret_line = line_nl;
		else
		{
			tmp = ret_line;
			ret_line = ft_strjoin(ret_line, line_nl);
			free(tmp);
			free(line_nl);
			if (!ret_line)
				return (-1);
		}
	}
	*out = ret_line; /* may be NULL if EOF/no lines */
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

t_cmd *lexer(t_token *token, t_cmd *cmd)
{
	t_token	*current;
	char	**cmd_tab;
	char	*heredoc;
	int		tab_idx;
	int		fd_in;
	int		fd_out;
	int		app_mode;
	int		words;
	t_cmd	*new_node;

	current = token;
	cmd_tab = NULL;
	tab_idx = 0;
	app_mode = 0;
	fd_in = -1;
	fd_out = -1;
	heredoc = NULL;
	while (current)
	{
		if (current->type == WORD)
		{
			if (!cmd_tab)
			{
				words = count_words_until_pipe(current);
				cmd_tab = ft_calloc(words + 1, sizeof(char *));
				if (!cmd_tab)
					return NULL;
				tab_idx = 0;
			}
			cmd_tab[tab_idx++] = ft_strdup(current->value);
			current = current->next;
			continue ;
		}
		if (!current->next)
			return (0);
		if (current->type == REDIRECT_APPEND)
		{
			fd_out = open(current->next->value, O_WRONLY | O_CREAT | O_APPEND,
					0644);
			if (fd_out == -1)
				return NULL; // a verif, il faut check ce qu'on fait quand un mauvais fichier est envoyer
			current = current->next->next;
			continue ;
		}
		else if (current->type == REDIRECT_HEREDOC)
		{
			if (do_heredoc(current->next->value, &heredoc) == -1)
				return NULL;
			current = current->next->next;
			continue ;
		}
		else if (current->type == REDIRECT_IN)
		{
			fd_in = open(current->next->value, O_RDONLY);
			if (fd_in == -1)
				return NULL;
			current = current->next->next;
			continue ;
		}
		else if (current->type == REDIRECT_OUT)
		{
			fd_out = open(current->next->value, O_WRONLY | O_CREAT | O_TRUNC,
					0644);  // est-ce qu'on create si il n'existe pas ?
			if (fd_out == -1)
				return NULL;
			current = current->next->next;
			continue ;
		}
		else if (current->type == PIPE)
		{
			new_node = cmd_create(cmd_tab, fd_in, fd_out, app_mode);
			if (!new_node)
				return NULL;
			new_node->heredoc = heredoc;
			heredoc = NULL;
			cmd_append(&cmd, new_node);
			cmd_tab = NULL;
			tab_idx = 0;
			fd_in = -1;
			fd_out = -1;
			app_mode = 0;
			current = current->next;
			continue ;
		}
		current = current->next;
	}
	if (cmd_tab)
	{
		new_node = cmd_create(cmd_tab, fd_in, fd_out, app_mode);
		if (!new_node)
			return NULL;
		new_node->heredoc = heredoc;
		cmd_append(&cmd, new_node);
	}
	return (cmd);
}

