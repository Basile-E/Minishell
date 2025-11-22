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


int	print_token(t_token *head)
{
	t_token	*temp;

	temp = head;
	if (temp == NULL)
		return (ft_printf("liste Vide\n"));
	while (temp)
	{   
		ft_printf("%s - ", temp->value);
        temp = temp->next;
	}
	return (ft_printf("\n"));
}
