/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 19:55:35 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 19:55:36 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	set_struct_minishell(t_minishell *minishell, char **env)
{
	minishell->input = NULL;
	minishell->status = DONT_KILL;
	minishell->alloc = NULL;
	ft_creat_env(minishell, env);
	return (1);
}
