/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_signal_handlers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 00:46:26 by kuru              #+#    #+#             */
/*   Updated: 2025/02/06 00:46:45 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ctl_c_handler_interactive(int sig)
{
	(void)sig;
	g_sig_offset = 130;
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	ctl_c_handler_exec(int sig)
{
	(void)sig;
	g_sig_offset = 130;
	write(1, "\n", 1);
	signal(SIGINT, ctl_c_handler_exec);
}

void	ctl_c_handler_heredoc(int sig)
{
	(void)sig;
	g_sig_offset = 130;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	ctl_back_handler(int sig)
{
	(void)sig;
	write(2, "Quit (core dumped)\n", 19);
	g_sig_offset = 131;
	signal(SIGQUIT, ctl_back_handler);
}
