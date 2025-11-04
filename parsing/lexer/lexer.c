#include "minishell.h"

/* I know a lexer is a parser, go touch grass */

t_cmd *cmd_create(char **cmds, int fd_in, int fd_out, int app_mode)
{
	t_cmd *new;
	int i;
	
	new = malloc(sizeof(t_cmd));
	i = 0;
	if(!new)
		return(NULL);
	while(cmds[i])
	{
		new->args[i] = ft_strdup(cmds[i]);
		i++;
	}
	new->fd_in = fd_in;
	new->fd_out = fd_out;
	new->append_mode = app_mode;
	new->next = NULL;
	new->heredoc = NULL;
	free(cmds);
	return (new);
}

void cmd_append(t_cmd **cmd, t_cmd *new_node)
{
	t_cmd *current;

	if (!*cmd)
	{
		*cmd = new_node;
		return;
	}
	current = *cmd;
	while(current->next)
		current = current->next;
	current->next = new_node;
}

void	print_lexer(t_cmd *cmd)
{
	t_cmd *current;

	current = cmd;
	
	while (current)
	{
		print_env(current->args);
		printf("%i\n", current->fd_in);
		printf("%i\n", current->fd_out);
		printf("%i\n", current->append_mode);
		printf("%s\n", current->heredoc);
		current = current->next;
	}
}

// est-ce qu'il faut absolument le charger dans un fd ? si oui est-ce que je dois le mettre dans mon fd in ou c'est un fd heredoc
char *do_heredoc(char *limiter)
{
	char *line;
	char *ret_line;
	char *line_nl;

	ret_line = NULL;
	while((line = readline("Theirdoc>")) != NULL)
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)))
			break;
		
		line_nl = ft_strjoin(line, "\n");
		free(line);
		if (!line_nl)
		{
			free(ret_line);
			return(NULL);
		}
		if (!ret_line)
			ret_line = line_nl;
		else
		{
			ret_line = ft_strjoin(ret_line, line);
			free(line_nl);
			if (!ret_line)
				return(NULL);
		}
	}
	return (ret_line);
}

static int count_words_until_pipe(t_token *start)
{
    int count = 0;
    while (start && start->type != PIPE)
    {
        if (start->type == WORD)
            count++;
        start = start->next;
    }
    return count;
}

int lexer(t_token *token, t_cmd *cmd)
{
	t_token *current;
	char **cmd_tab;
	char *heredoc;
	int		tab_idx;
	int	fd_in;
	int fd_out;
	int app_mode;
	int words;

	current = token;
	tab_idx = 0;
	app_mode = 0;
	fd_in = 0;
	fd_out = 0;
	heredoc = NULL;
	while (current)
	{
		words = count_words_until_pipe(token);
		if (current->type == WORD)
			cmd_tab[tab_idx] = ft_strdup(current->value);
		else // on a trouver un sep, youpi !
		{
			if (current->type == REDIRECT_APPEND)
			{
				app_mode = 1;
				fd_out = open(current->next->value, O_WRONLY);
				if(!fd_out)
					return(0); // normalement on continu vers l'exec anyway et on doit bloquer sur le fichier manquant
			}
			else if (current->type == REDIRECT_HEREDOC)
			{
				heredoc = do_heredoc(current->next->value);
				if (!heredoc)
					return(0);
			}
			else if (current->type == REDIRECT_IN)
			{
				fd_in = open(current->next->value, O_RDONLY);
				if (!fd_in)
					return(0);
			}
			else if (current->type == REDIRECT_OUT)
			{
				fd_out = open(current->next->value, O_WRONLY);
				if (!fd_out)
					return(0);
			}
			if(current->type == PIPE || current->next == NULL)
				cmd_append(&cmd, cmd_create(cmd_tab, fd_in, fd_out, app_mode));
		}
		current = current->next;
		tab_idx++;
	}
	printf("i got here\n\n\n");
	print_lexer(cmd);
	return(1);
}



// 1 create token cmd
// 2 append cmd

// token_to_cmd() : 
// parcours la liste,
// 	premier elem doit etre un mot, il deviens la cmd, 
// 	puis jusquq qu'un operateur soir found il ajoute des lignes au ret_tab,
//  		quand op is found
// 			check de pipe ou redirection si pipe on stop ici et passe a la prochaine cmd 
// 			else
//  				on trouve le "sens" de la redirection et on set les params de la struct puis on passe a la prochaine cmd