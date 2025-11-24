/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_check_next.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:04:45 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 20:59:45 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (1);
	return (0);
}

int	is_sep(char c)
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '|' || c == '>' || c == '<'
		|| c == '\0')
		return (1);
	return (0);
}

int	is_space(char c)
{
	if (c == ' ' || (c >= '\t' && c <= '\v'))
		return (1);
	return (0);
}

int	is_lowercase(char suspect)
{
	if (suspect >= 'a' && suspect <= 'z')
		return (1);
	return (0);
}

int	is_uppercase(char suspect)
{
	if (suspect >= 'A' && suspect <= 'Z')
		return (1);
	return (0);
}
