#include "../includes/minishell.h"

void print_env(char **env)
{
    int i;
    i = 0;

    while (env[i])
    {
        printf("%s\n", env[i]);
        i++;
    }
}


t_args *create_and_add_token(t_args **head, t_args **current, char *content)
{
    t_args *new_node = malloc(sizeof(t_args));
    if (!new_node)
        return (NULL);
    
    new_node->arg = ft_strdup(content);
    new_node->next = NULL;
    
    // Si c'est le premier nœud
    if (!*head)
    {
        *head = new_node;
        *current = new_node;
    }
    else
    {
        // Ajouter à la fin de la liste
        (*current)->next = new_node;
        *current = new_node;
    }
    
    return (new_node);
}

int is_operator(char c)
{
    if (c == '|' || c == '>'  || c == '<' || 
        c == '"' || c == '\'')
        return (1);
    else 
        return (0);
}

int is_sep(char c)
{
    if (c == ' ' || c == '\n' || c == '\t' || c == '"' ||
        c == '\''|| c == '|'  || c == '>'  || c == '<' || c == '\0') //pas sur que \0 soit un sep
        return (1);
    else 
        return (0);
}


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


int tab_len(char **tab)
{
    int i;
    i = 0;

    while(tab[i] != NULL)
        i++;
    return (i);
}
/*
    j'ai piquer ma fonction find path de pipex 
    il faut la custom un peut pour qu'elle cherche n'importe quelle env value
*/

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

char *do_expand(t_minishell *minishell, char *str)
{
    char *token;
    char *env_value;
    int i;
    int y;
    

    token = ft_strdup(str + check_for_expand(str));
    if (!token)
        return (NULL);
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

int	print_token(t_args *head)
{
	t_args	*temp;

	temp = head;
	if (temp == NULL)
		return (ft_printf("liste Vide\n"));
	while (temp)
	{   
		ft_printf("%s - ", temp->arg);
        temp = temp->next;
	}
	return (ft_printf("\n"));
}

void clear_buff(char *buffer)
{
    int i;
    i = ft_strlen(buffer);

    while (i >= 0)
    {
        buffer[i] = '\0';
        i--;
    }
}



int parsinette(t_minishell *minishell, t_args **args)
{
    t_args *current;

    write(1, "hi from parsinette !\n", 21);
    current = *args;
    while (current)
    {
        current->arg = do_expand(minishell, current->arg);
        
        
        
        current = current->next;
    }
    return (0);
    
}

int tokenizer(t_minishell *minishell, t_args **args)
{
    int i; // sert a se deplacer dans argument
    char *arguments;
    char buff[BUFF_SIZE];
    int buff_pos; // sert a write et a se deplacer dans le buffer 
    t_args *current;
    char operator[2];
    
    i = 0;
    buff_pos = 0;
    arguments = minishell->input;
    current = *args;
    if (!arguments || arguments[0] == '\0')
        return (0);
    while (arguments[i])
    {
        if (is_sep(arguments[i]))
        {
            if (buff_pos > 0)
            {
                buff[buff_pos] = '\0'; // nul terminate, permet de "reset" le buf by seting the buf_pos to 0
                current = create_and_add_token(args, &current, buff);
                buff_pos = 0;
            }
            operator[0] = arguments[i];
            operator[1] = '\0';
            current = create_and_add_token(args, &current, operator);
            current->type = OPERATOR;
        }
        else
        {
            if (buff_pos < BUFF_SIZE - 1)
            {
                buff[buff_pos] = arguments[i];
                buff_pos++;
            }
        }
        i++;
    }
    if (buff_pos > 0)
    {
        buff[buff_pos] = '\0';
        current = create_and_add_token(args, &current, buff);
    }
    print_token(*args);
    return (1);
}

int set_struct(t_minishell *minishell, char **env)
{
    minishell->input = NULL;
    minishell->status = DONT_KILL;
    minishell->env = env;
    return (1);
}

int main(int ac, char **av, char **ev)
{
    (void)ac;
    (void)av;

    char *prompt;
    t_minishell *minishell;
    t_args *args;

    minishell = malloc(sizeof(t_minishell));
    args = NULL;
    prompt = BOLD CYAN "RicoShell" RESET PINK " ➜ " RESET;
    
    set_struct(minishell, ev);
    while (42)
    {
        if (minishell->status == KILL_SIM)
            break;

        minishell->input = readline(prompt);
        if (!minishell->input) // est-ce que readline peut fail ?
            return(printf("Exit\n"), 0);
        add_history(minishell->input);
        printf("Debug :\nString sent by readline : %s\n", minishell->input);
        args = NULL;
        if (!tokenizer(minishell, &args))
            printf("Tokenizer Failed\n"); // le tout est un peut sale mais c'est une base pour gerer les retours d'erreurs (plusieurs edges cases a gerer : user sent \n -> on print le retour a la ligne (arg sera == a \n), user sent too much char ou une cmd illegal -> print un msg d'erreur standard type cmd illegal cmd name to long)
        else
            printf("\n");
        parsinette(minishell, &args);
        print_token(args); // sert a debug
    }
    return(0);
}


// compiling cmd : cc -g3 -Wall -Werror -Wextra main.c -L./Libft -lft -lreadline -o RicoShell
