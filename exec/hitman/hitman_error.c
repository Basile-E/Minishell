/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hitman_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emle-vou <emle-vou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 21:53:51 by emle-vou          #+#    #+#             */
/*   Updated: 2025/11/23 21:56:50 by emle-vou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}

char	*get_current_path(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		return (ft_error("getcwd", strerror(errno)), NULL);
	return (path);
}

void	put_err_msg(char *str)
{
	ft_putstr_fd("Error: command not found: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
}
