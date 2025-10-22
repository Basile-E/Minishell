#include "minishell.h"

// check if c is an operator or a separator (without distinguishing one from the other)

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
    retun (i);
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

// il faut toujours ajouter les sep dans des token au moment de vider le buffer 
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
            if (is_operator(arguments[i]))
            {
                operator[0] = arguments[i];
                operator[1] = '\0';
                current = create_and_add_token(args, &current, operator);
                current->type = OPERATOR;
            }   
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

int set_struct(t_minishell *minishell)
{
    minishell->input = NULL;
    minishell->status = DONT_KILL;
    return (1);
}




int main(int ac, char **av, char **ev)
{
    char *prompt;
    t_minishell *minishell;
    t_args *args;
    minishell = malloc(sizeof(t_minishell));
    args = NULL;
    set_struct(minishell);
    prompt = BOLD CYAN "RicoShell" RESET PINK " ➜ " RESET;

    /*
        DEBUG 
    */


    
    /*
        DEBUG 
    */


    while (42)
    {
        if (minishell->status == KILL_SIM)
            break;

        minishell->input = readline(prompt);
        if (!minishell->input) // est-ce que readline peut fail ?
            return(printf("Readline Failed\n"), 1);
        add_history(minishell->input);
        printf("Debug :\nString sent by readline : %s\n", minishell->input);
        args = NULL;
        if (!tokenizer(minishell, &args))
            printf("Tokenizer Failed\n"); // le tout est un peut sale mais c'est une base pour gerer les retours d'erreurs (plusieurs edges cases a gerer : user sent \n -> on print le retour a la ligne (arg sera == a \n), user sent too much char ou une cmd illegal -> print un msg d'erreur standard type cmd illegal cmd name to long)
        else
            printf("\n");
        if (!ft_strncmp(minishell->input, "kill", 4))
            minishell->status = KILL_SIM; // c'est ici qu'on vas tester la fonction clean and quit (ou est-ce que ca sert a rien, en vrais bash a pas de kill feature pour lui meme)
    }
    return(0);
}


// compiling cmd : cc -g3 -Wall -Werror -Wextra main.c -L./Libft -lft -lreadline -o RicoShell
