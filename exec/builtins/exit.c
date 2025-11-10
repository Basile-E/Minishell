/*
** exit builtin
** behavior:
**  - exit [n]
**  - if n is not numeric -> print error and exit 255
**  - if more than one arg and first is numeric -> print "too many arguments" and return 1 (do not exit when in parent)
**  - if called in child (in_child == 1) -> call _exit(status)
*/

#include "../../includes/minishell.h"
#include <stdlib.h>
#include <errno.h>

static int	is_num(const char *str)
{
	int i = 0;

	if (!str || !*str)
		return (0);
	if (str[0] == '+' || str[0] == '-')
		i = 1;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	to_exit_status(const char *str)
{
	int val;

	if (!str)
		return (0);
	val = ft_atoi(str);
	return (val & 0xFF);
}

int	ft_exit(char **argv, t_minishell *mini, int in_child)
{
	int status = 0;

	(void)mini;
	ft_putendl_fd("exit", 1);
	if (!argv || !argv[1])
	{
		if (in_child)
			_exit(0);
		exit(0);
	}
	if (!is_num(argv[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		if (in_child)
			_exit(255);
		exit(255);
	}
	if (argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	status = to_exit_status(argv[1]);
	if (in_child)
		_exit(status);
	exit(status);
}