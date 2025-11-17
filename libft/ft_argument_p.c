/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_argument_p.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 19:00:10 by baecoliv          #+#    #+#             */
/*   Updated: 2025/02/18 21:04:37 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_argument_p(unsigned long address)
{
	char	*num;
	int		bytes;
	int		i;

	i = 0;
	if (!address)
		return (write(1, "(nil)", sizeof(char) * 5));
	bytes = write(1, "0x", sizeof(char) * 2);
	num = (char *)malloc(((ft_hex_length(address)) + 1) * sizeof(char));
	while (address)
	{
		num[i] = ft_decimal_converter_to_hex(address % 16, 'x');
		address /= 16;
		i++;
	}
	num[i] = '\0';
	bytes += ft_print_reversed_str(num);
	ft_free_ptr(&num);
	return (bytes);
}
