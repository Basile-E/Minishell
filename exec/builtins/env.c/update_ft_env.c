#include "minishell.h"

void	ft_update_env(t_hub *hub, t_minishell *mini, char *new)
{
	int		i;
	char	*dst;

	if (verif_value(new) != 0)
		return ;
	if (!mini->env)
		return (ft_add_env(hub, mini, new));
	i = 0;
	while (mini->env[i])
	{
		if (ft_strcmp_equal(new, mini->env[i]) == 0)
			break ;
		i++;
	}
	if (!mini->env[i])
		return (ft_add_env(hub, mini, new));
	free(mini->env[i]);
	dst = malloc(ft_strlen(new) + 1);
	if (!dst)
		return (ft_error("update env", strerror(errno)), ft_exit(hub, 1));
	ft_strcpy(dst, new);
	mini->env[i] = dst;
}

