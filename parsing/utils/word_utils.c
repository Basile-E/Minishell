/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 19:42:48 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/24 19:56:46 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_word_token(t_minishell *minishell, t_token *token)
{
	char	*expanded;
	char	*cleaned;

	expanded = do_expand_simple(minishell, token->value);
	(void)cleaned;
	free(token->value);
	token->value = expanded;
	return (1);
}
