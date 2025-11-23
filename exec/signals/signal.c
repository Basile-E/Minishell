/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emle-vou <emle-vou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 19:57:50 by emle-vou          #+#    #+#             */
/*   Updated: 2025/11/23 19:18:56 by emle-vou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

volatile sig_atomic_t g_signal = 0;

void	handle_normal_sigint(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_exec_sigint(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	handle_heredoc_sigint(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

void	handle_sigquit(int sig)
{
	write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	g_signal = sig;
}

void	handle_sigchld(int sig)
{
	while (waitpid(-1, NULL, WNOHANG) > 0)
		g_signal = sig;
}
