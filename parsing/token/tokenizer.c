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

int check_unclosed_quotes(char *str)
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
