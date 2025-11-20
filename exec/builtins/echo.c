#include "minishell.h"

static int	ft_n_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	i = 1;
	if (str[i] != 'n')
		return (0);
	while (str[i] == 'n')
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

int	ft_echo(t_minishell mini, char **argv)
{
	int	i;
	int	n_flag;

	(void) mini;
	if (!argv)
		return (0);
	i = 1;
	n_flag = 1;
	while (argv[i] && ft_n_flag(argv[i]))
	{
		n_flag = 0;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (n_flag)
		ft_putstr_fd("\n", 1);
	return (0);
}
