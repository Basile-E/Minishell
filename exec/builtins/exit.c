#include "minishell.h"

static long long	ft_atoi_2(char *src)
{
	int			i;
	int			s;
	long long	res;

	i = 0;
	s = 0;
	if (src[i] == '-')
		s = 1;
	if (src[i] == '-' || src[i] == '+')
		i++;
	res = 0;
	while (src[i] >= '0' && src[i] <= '9')
	{
		res = res * 10 + (src[i] - '0');
		i++;
	}
	if (s == 1)
		return (res * -1);
	return (res);
}

static int	verif_long_long_2(char *nbr)
{
	int		i;
	int		j;
	char	*longlong;

	i = 0;
	j = 0;
	longlong = "9223372036854775807";
	if (nbr[i] == '+')
		longlong = "+9223372036854775807";
	while (nbr[i] && longlong[i])
	{
		if (nbr[i] > longlong[i])
			j++;
		i++;
	}
	if ((longlong[i] == 0 && j > 0) || (longlong[i] == 0 && nbr[i] != 0))
		return (1);
	return (0);
}

static int	verif_long_long(char *nbr)
{
	int		i;
	int		j;
	char	*longlong_ng;

	i = 0;
	j = 0;
	longlong_ng = "-9223372036854775808";
	if (nbr[i] == '-')
	{
		while (nbr[i] && longlong_ng[i])
		{
			if (nbr[i] > longlong_ng[i])
				j++;
			i++;
		}
		if ((longlong_ng[i] == 0 && j > 0) || (longlong_ng[i] == 0 && nbr[i] != 0))
			return (1);
		return (0);
	}
	return (verif_long_long_2(nbr));
}

static int	nbr_exit(char **argv, t_minishell *mini, char *nbr)
{
	int i;

	if (nbr == NULL)
		return (0);
	if (nbr[0] == '\0')
	{
		mini->status = 2;
		return (ft_error("exit", "numeric argument required"), 0);
	}
	i = 0;
	if (nbr[i] == '+' || nbr[i] == '-')
		i++;
	while (nbr[i] && (nbr[i] >= '0' && nbr[i] <= '9'))
		i++;
	if (nbr[i] != '\0')
	{
		mini->status = 2;
		return (ft_error("exit", "numeric argument required"), 0);
	}
	if (verif_long_long(nbr) != 0)
	{
		mini->status = 2;
		return (ft_error("exit", "numeric argument required"), 0);
	}
	if (argv && argv[2] != NULL)
		return (ft_error("exit", "too many arguments"), 1);
	mini->status = (unsigned char)ft_atoi_2(nbr);
	return (0);
}
