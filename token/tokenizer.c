#include "../includes/minishell.h"


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

/*
	create a buff with buff size == 300
	look for quote in the arguments 
		return 1 if in double quote, 2 if signle quote, 0 if no quote and -1 if quote error
	fill the buff until a sep is found
		sep edge case :
			if (space)
				if (in_quote)
					add the space to buff
				else
					ignore it
			if (|)
				
*/

void set_int(int *a, int *b, int *c, int *d)
{
	if (a != NULL)
		*a = 0;
	if (b != NULL)
		*b = 0;
	if (c != NULL)
		*c = 0;
	if (d != NULL)
		*d = 0;
}

int find_quote(char *str)
{
	int i;
	int in_double;
	int in_simple;
	int in_quote;
	set_int(&i, &in_double, &in_simple, &in_quote);
	
	while (i < 0)
	{
		if (str[i] == '"' && in_quote == 0)
		{
			in_quote = 1;
			in_double = 1;
		}
		else if (str[i] == '"' && in_quote == 1)
		{
			
		}
		if (str[i] == '\'' && in_quote == 0)
		{
			in_quote = 1;
			in_simple = 1;
		}
		i++;
	}
}

int tokenizerV2(t_minishell *minishell, t_args **args)
{
	
}
