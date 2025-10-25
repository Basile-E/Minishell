/*built-ins incluent
		- echo -n
		- cd relative or absolute path
		- pwd
		- export
		- unset
		- env
		- exit */

// on regarde si le n flag est la, il va ajouter un '/n'
// a la ligne renvoye par minishell

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
	/* if we've reached end of string, it's a valid -n, -nn... flag */
	if (str[i] == '\0')
		return (1);
	return (0);
}

int	ft_echo(char **argv)
{
	int i;
	int n_flag;

	if (!argv)
		return (0);
	i = 1;
	n_flag = 1;
	/* if there are -n / -nn flags, disable trailing newline */
	while (argv[i] && ft_n_flag(argv[i]))
	{
		n_flag = 0;
		i++;
	}
	/* print remaining args separated by spaces */
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	/* print newline unless -n was present */
	if (n_flag)
		ft_putstr_fd("\n", 1);
	return (0);
}