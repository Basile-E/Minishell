/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 16:40:26 by baecoliv          #+#    #+#             */
/*   Updated: 2024/09/07 17:07:38 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int argument)
{
	int		length;

	length = ft_strlen(str);
	while (length >= 0)
	{
		if ((unsigned char) str[length] == (unsigned char)argument)
			return ((char *)&str[length]);
		length--;
	}
	return (NULL);
}
