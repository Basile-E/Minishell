#include "minishell.h"

int	ft_env(t_minishell *mini)
{
	int	i;

<<<<<<< HEAD
	if (!mini->env)
		return(0);
=======
>>>>>>> Merging
	i = 0;
	while (mini->env[i])
	{
		ft_putstr_fd(mini->env[i], 1);
<<<<<<< HEAD
		ft_putchar_fd('\n', 1);
=======
>>>>>>> Merging
		i++;
	}
	return (0);
}