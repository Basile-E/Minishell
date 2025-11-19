/*Si plus que env alors faire un return 2 et envoyer un message d'erreur \
D'abord je recup envp et je le copie dans un double tableau
ATTENTION je ne vais pas trier la commande export car ce n'est precise ni dans le sujet 
Pas besoin de creer de double tableau (choix) donc impossibilite de trier */

#include "minishell.h"
<<<<<<< HEAD

// void update_SHLVL(t_minishell *mini)
// {
// 	int 	lv;
// 	int		i;

// 	i = 0;
// 	while (mini->env[i] && ft_strnstr(mini->env[i], "SHLVL", 5) == 0)
// 		i++;
// 	lv = ft_atoi(mini->env[i] + 6);
// 	return;
// }

=======
>>>>>>> Merging
static void	error_env(t_minishell *mini, int i)
{
	while (i >= 0)
	{
		free(mini->env[i]);
		mini->env[i] = NULL;
		i--;
	}
	free(mini->env);
	mini->env = NULL;
}

<<<<<<< HEAD
void	ft_creat_env(t_minishell *mini, char **envp)
=======
void	ft_creat_env(t_hub *hub, t_minishell *mini, char **envp)
>>>>>>> Merging
{
	int	i;

	if (!envp || !envp[0])
<<<<<<< HEAD
		return;
	mini->env = malloc(sizeof(char *) * (ft_strlen_y(envp) + 1));
	if (!mini->env)
		return;
=======
		return (0);
	mini->env = malloc(sizeof(char *) * (ft_strlen_y(envp) + 1));
	if (!mini->env)
		return (0);
>>>>>>> Merging
	i = 0;
	while (envp[i])
	{
		mini->env[i] = malloc(ft_strlen(envp[i]) + 1);
		if (!mini->env[i])
		{
			error_env(mini, i - 1);
<<<<<<< HEAD
			return;
		}
		ft_strlcpy(mini->env[i], envp[i], ft_strlen(envp[i]) + 1);
=======
			return (0);
		}
		ft_strcpy(mini->env[i], envp[i]);
>>>>>>> Merging
		i++;
	}
	mini->env[i] = NULL;
}

<<<<<<< HEAD
void	ft_add_env(t_minishell *mini, char *new)
=======
void	ft_add_env(t_hub *hub, t_minishell *mini, char *new)
>>>>>>> Merging
{
	int		i;
	char	**new_mini;
	char	*dst;

	if (!mini->env)
<<<<<<< HEAD
		return;
	new_mini = malloc(sizeof(char *) * (ft_strlen_y(mini->env) + 2));
	if (!new_mini)
		return;
=======
		return (0);
	new_mini = malloc(sizeof(char *) * (ft_strlen_y(mini->env) + 2));
	if (!new_mini)
		return (0);
>>>>>>> Merging
	i = 0;
	while (mini->env[i])
	{
		new_mini[i] = mini->env[i];
		i++;
	}
	dst = malloc(ft_strlen(new) + 1);
	if (!dst)
	{
		free(new_mini);
<<<<<<< HEAD
		return;
	}
	ft_strlcpy(dst, new, ft_strlen(new) + 1);
	new_mini[i] = dst;
	new_mini[i + 1] = NULL;
	free(mini->env);
	mini->env = new_mini;
	return;
=======
		return (FT_ERROR);
	}
	ft_strcpy(dst, new);
	new_mini[i] = dst;
	new_mini[i + 1] = NULL;
	return (free(mini->env), mini->env = new_mini, ft_void());
>>>>>>> Merging
}
