/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baecoliv <baecoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 21:57:28 by baecoliv          #+#    #+#             */
/*   Updated: 2025/11/23 23:21:15 by baecoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	be_a_heredoc(char **line, char **ret_line, char **limiter, char **line_nl)
{
	char	*tmp;

	if (ft_strncmp(*line, *limiter, ft_strlen(*limiter)) == 0)
	{
		free(*line);
		return (0);
	}
	*line_nl = ft_strjoin(*line, "\n");
	free(*line);
	if (!*line_nl)
		return (free(ret_line), -1);
	if (!*ret_line)
		*ret_line = *line_nl;
	else
	{
		tmp = *ret_line;
		*ret_line = ft_strjoin(*ret_line, *line_nl);
		free(tmp);
		free(*line_nl);
		if (!*ret_line)
			return (-1);
	}
	return (1);
}

int	do_heredoc(char *limiter, char **out)
{
	t_heredoc	her;

	if (!limiter || !out)
		return (-1);
	if (*out)
		free(*out);
	her.ret_line = NULL;
	her.line = readline("Theredoc>");
	while (her.line)
	{
		her.here_ret = be_a_heredoc(&her.line, &her.ret_line, &limiter,
				&her.line_nl);
		if (her.here_ret == 0)
			break ;
		if (her.here_ret == -1)
			return (-1);
		her.line = readline("Theredoc>");
	}
	if (!her.line)
		printf("minishell: warning: here-document\n");
	*out = her.ret_line;
	if (!*out)
		*out = ft_strdup("");
	return (0);
}
