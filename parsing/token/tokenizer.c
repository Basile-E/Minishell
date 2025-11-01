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

int check_unclosed(char *str)
{
	int i;
	t_quote	status;

	i = 0;
	status = NONE;
	while(str[i])
	{
		if (status == NONE && str[i] == '\'')
			status = SINGLE;
		else if (status == NONE && str[i] == '"')
			status = DOUBLE;
		else if ((status == SINGLE && str[i] == '\'') ||
				 (status == DOUBLE && str[i] == '"'))
			status = NONE;
		i++;
	}
	return((int)status);
}


// c'est ok manque juste les doubles opperateur 
// je peux en faire un check dans le parsing

int tokenizer(t_minishell *minishell, t_args **args)
{
    int i; // sert a se deplacer dans argument
    char *arguments;
    char buff[BUFF_SIZE];
    int buff_pos; // sert a write et a se deplacer dans le buffer 
    t_args *current;
    char operator[2];
    t_quote status;


    i = 0;
    buff_pos = 0;
    arguments = minishell->input;
    current = *args;
	status = NONE;
	if (!arguments || arguments[0] == '\0' || check_unclosed(arguments))
        return (0);
    while (arguments[i])
    {
		if (status == NONE)
        {
			if (arguments[i] == '\'')
				status = SINGLE;
			else if (arguments[i] == '"')
				status = DOUBLE;
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
		}
		else
		{
			if ((arguments[i] == '"' && status == DOUBLE) ||
				(arguments[i] == '\'' && status == SINGLE))
				status = NONE;
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
    //print_token(*args);
    return (1);
}
