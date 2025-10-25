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
