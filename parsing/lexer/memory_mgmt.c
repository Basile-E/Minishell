/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_mgmt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 21:54:28 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 20:41:43 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup_gc(char *src, t_minishell *mini)
{
	size_t	len;
	char	*dup;

	if (!src)
		return (NULL);
	len = ft_strlen(src);
	dup = ft_malloc((int)(len + 1), &mini->alloc);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, src, len);
	dup[len] = '\0';
	return (dup);
}

void	*ft_calloc_gc(size_t nbr_elements, size_t element_size,
		t_minishell *mini)
{
	void	*ptr;

	ptr = ft_malloc(nbr_elements * element_size, &mini->alloc);
	if (ptr == NULL)
		return (NULL);
	ft_bzero(ptr, nbr_elements * element_size);
	return (ptr);
}

char	*ft_strjoin_gc(const char *s1, const char *s2, t_minishell *mini)
{
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	str = (char *)ft_calloc_gc((ft_strlen(s1) + ft_strlen(s2)) + 1,
			sizeof(char), mini);
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, ft_strlen(s1) + 1);
	ft_strlcat(str, s2, ft_strlen(s1) + ft_strlen(s2) + 1);
	return (str);
}
