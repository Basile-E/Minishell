#include "../includes/minishell.h"

/*
				Pseudo :
				persing of the input string until '\0'
					if a sep is found
						check that buff isnt empty
							nul terminate the buff
							check if that is the first node of the list
								if it is maloc the temp node and the arg node
							duplicate the content of buff into the node
							set the temp node to the next one
							set buf_pos to 0 to reset the buff

					else
						while buff_pos < BUFF_SIZE
						buff[buff_pos] = argument[i]
						i++;
				check si le buff_pos n'est pas a 0
					check de si c'est le premier noeud
					copie de ce qu'il reste dans le buff dans un arg

*/
// faire une fonction create token qui prend en arg le noeud a remplir et la string (plus d'autre param si on veut en ajouter apres)

/*
	j'ai piquer ma fonction find path de pipex
	il faut la custom un peut pour qu'elle cherche n'importe quelle env value
*/

int	main(int ac, char **av, char **ev)
{
	char		*prompt;
	t_minishell	*minishell;
	t_args		*args;

	(void)ac;
	(void)av;
	minishell = malloc(sizeof(t_minishell));
	args = NULL;
	prompt = BOLD CYAN "RicoShell" RESET PINK " ➜ " RESET;
	set_struct_minishell(minishell, ev);
	while (42)
	{
		if (minishell->status == KILL_SIM)
			break ;
		minishell->input = readline(prompt);
		if (!minishell->input) // est-ce que readline peut fail ?
			return (printf("Exit\n"), 0);
		add_history(minishell->input);
		printf("Debug :\nString sent by readline : %s\n", minishell->input);
		args = NULL;
		if (!tokenizer(minishell, &args))
			printf("Tokenizer Failed\n");
				// le tout est un peut sale mais c'est une base pour gerer les retours d'erreurs (plusieurs edges cases a gerer : user sent \n
				-> on print le retour a la ligne (arg sera == a \n),
				user sent too much char ou une cmd illegal
				-> print un msg d'erreur standard type cmd illegal cmd name to long)
		else
			printf("\n");
		parsinette(minishell, &args);
		print_token(args); // sert a debug
	}
	return (0);
}

// compiling cmd : cc -g3 -Wall -Werror -Wextra main.c -L./Libft -lft -lreadline
	-o RicoShell
