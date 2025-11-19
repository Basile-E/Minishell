#include "minishell.h"

<<<<<<< HEAD
void	ft_update_env(t_minishell *mini, char *new)
=======
void	ft_update_env(t_hub *hub, t_minishell *mini, char *new)
>>>>>>> Merging
{
	int		i;
	char	*dst;

<<<<<<< HEAD
	// if (verif_value(new) != 0) // check si lq value existe deja ?
	// 	return ;
	if (!mini->env)
		return (ft_add_env( mini, new));
=======
	if (verif_value(new) != 0)
		return ;
	if (!mini->env)
		return (ft_add_env(hub, mini, new));
>>>>>>> Merging
	i = 0;
	while (mini->env[i])
	{
		if (ft_strcmp_equal(new, mini->env[i]) == 0)
			break ;
		i++;
	}
	if (!mini->env[i])
<<<<<<< HEAD
		return (ft_add_env( mini, new));
	free(mini->env[i]);
	dst = malloc(ft_strlen(new) + 1);
	if (!dst)
		return (ft_error("update env", strerror(errno)), _exit(1));
	ft_strlcpy(dst, new, ft_strlen(new) + 1);
=======
		return (ft_add_env(hub, mini, new));
	free(mini->env[i]);
	dst = malloc(ft_strlen(new) + 1);
	if (!dst)
		return (ft_error("update env", strerror(errno)), ft_exit(hub, 1));
	ft_strcpy(dst, new);
>>>>>>> Merging
	mini->env[i] = dst;
}

