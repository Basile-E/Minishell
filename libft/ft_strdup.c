/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/11/24 21:57:38 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 21:57:38 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(char *src)
{
	int		i;
	char	*dup;

	i = 0;
	dup = (char *)malloc((ft_strlen(src) + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	while (src[i])
	{
		dup[i] = src[i];
		++i;
	}
	dup[i] = '\0';
	return (dup);
}
