#include "minishell.h"

int	ft_env(t_minishell *mini)
{
	int	i;

	if (!mini->env)
		return(0);
	i = 0;
	while (mini->env[i])
	{
		ft_putstr_fd(mini->env[i], 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
	return (0);
}