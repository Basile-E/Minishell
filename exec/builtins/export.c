/*export permet d'ajouter ou modifier une variable avec une valeur "XX=xxx".
La variable apparait desormais quand on utilise 
les commandes env et export

Avec export seul on va faire apparaitre les variables dans 
l'ordre alphabetique. 

On va aussi verifier la variable donnee afin de voir si elle existe deja
ou si on doit la creer. */

#include "minishell.h"

static void error_exp(char **exp, int i)
{
	while (i >= 0)
	{
		free(exp[i]);
		exp[i] = NULL;
		i--;
	}
	free(exp);
}

char	**ft_create_export(t_minishell *mini)
{
	int	i;
	char **exp;

	exp = malloc(sizeof(char *) * (ft_strlen_y(mini->env) + 1));
	if (!mini->env)
		return (0);
	i = 0;
	while (mini->env[i])
	{
		exp[i] = malloc(ft_strlen(mini->env[i]) + 1);
		if (!exp[i])
		{
			error_exp(exp, i - 1);
			return (NULL);
		}
		ft_strlcpy(exp[i], mini->env[i], ft_strlen(mini->env[i]) + 1);
		i++;
	}
	exp[i] = NULL;
	return (exp);
}

void 	ft_sort_export(char **exp)
{
	int		i;
	int		j;
	char	*swap;

	j = 0;
	i = 0;
	while(exp[i])
	{
		j = 1;
		while (exp[j])
		{
			if (ft_strcmp_equal(exp[j - 1], exp[j]) > 0)
			{
				swap = exp[j - 1];
				exp[j - 1] = exp[j];
				exp[j] = swap;
			}
			j++;
		}
		i++;
	}
}

void	ft_export(t_minishell *mini)
{
	int	i;
	char **exp;

	i = 0;
	exp = ft_create_export(mini);
	ft_sort_export(exp);
	if (!exp)
		return;
	while (exp[i])
	{
		ft_putstr_fd("export ", 1);
		ft_putstr_fd(exp[i], 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
}