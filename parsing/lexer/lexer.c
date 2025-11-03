#include "minishell.h"

/* I know a lexer is a parser, go touch grass */

t_cmd *cmd_create(char **cmds, char *in_file, char *out_file, int app_mode)
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
	new->input_file = ft_strdup(in_file);
	new->output_file = ft_strdup(out_file);
	new->append_mode = app_mode;
	new->next = NULL;
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
		printf("%s\n", current->input_file);
		printf("%s\n", current->output_file);
		printf("%i\n", current->append_mode);
		current = current->next;
	}
}
int lexer(t_token *token, t_cmd *cmd)
{
	t_token *current;
	char **cmd_tab;
	int		tab_idx;
	char *in_file;
	char *out_file;
	int app_mode;

	current = token;
	tab_idx = 0;
	app_mode = 0;
	in_file = NULL;
	out_file = NULL;
	cmd_tab = NULL;
	while (current)
	{
		if (current->type == WORD)
			cmd_tab[tab_idx] = ft_strdup(current->value);
		else // on a trouver un sep, youpi !
		{
			if (current->type == REDIRECT_APPEND)
			{
				app_mode = 1;
				out_file = ft_strdup(current->next->value);
			}
			else if (current->type == REDIRECT_HEREDOC)
			{
				// ici il faut une fonction do_heredoc mais no idea de ce quelle return
			}
			else if (current->type == REDIRECT_IN)
			{
				in_file = ft_strdup(current->next->value);
			}
			else if (current->type == REDIRECT_OUT)
			{
				out_file = ft_strdup(current->next->value);
			}
			cmd_append(&cmd, cmd_create(cmd_tab, in_file, out_file, app_mode));
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