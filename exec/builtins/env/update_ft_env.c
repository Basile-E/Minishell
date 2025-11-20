#include "minishell.h"

void	ft_update_env(t_minishell *mini, char *new)
{
	int		i;
	char	*dst;

	// if (verif_value(new) != 0) // check si lq value existe deja ?
	// 	return ;
	if (!mini->env)
		return (ft_add_env( mini, new));
	i = 0;
	while (mini->env[i])
	{
		if (ft_strcmp_equal(new, mini->env[i]) == 0)
			break ;
		i++;
	}
	if (!mini->env[i])
		return (ft_add_env( mini, new));
	free(mini->env[i]);
	dst = ft_malloc(ft_strlen(new) + 1, &mini->alloc);
	if (!dst)
		return (ft_error("update env", strerror(errno)), _exit(1));
	ft_strlcpy(dst, new, ft_strlen(new) + 1);
	mini->env[i] = dst;
}
