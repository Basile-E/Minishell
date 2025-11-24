/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:12:50 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 20:12:51 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int is_number(char mightbeanint)
{
	if (mightbeanint >= '0' && mightbeanint <= '9')
		return(1);
	return(0);
}
int	is_char(char suspect)
{
	if (is_lowercase(suspect) || is_uppercase(suspect) ||
		is_number(suspect) || suspect == '_')
		return(1);
	return(0);
}
