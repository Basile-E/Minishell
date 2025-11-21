
#include "minishell.h"

int	ft_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		return (ft_error("getcwd", strerror(errno)), 1);
	ft_printf("%s\n", path);
	free(path);
	return (0);
}
