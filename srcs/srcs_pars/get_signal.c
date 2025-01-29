/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_signal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 11:57:23 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/29 14:05:54 by jbaumfal         ###   ########.fr       */
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
	rl_redisplay();
	ft_printf("\n");
	g_sig_offset = 130;
	rl_replace_line("", 0);
	rl_on_new_line();
	signal(SIGINT, ctl_c_handler_exec);
}

void ctl_c_handler_heredoc(int sig)
{
    ft_printf("handeling ctl c as heredoc\n");
	(void)sig;
    write(1, "\n", 1);
	// rl_replace_line("", 0);
	// rl_on_new_line();
	// rl_redisplay();
    exit(130);
}

/**
 * currently we dont use this function as
 *  we can ignore the signal directly in the get_signal function
 * if we need to change the g_sig_offset value to 131,
 * 	we can use this function and insert it in signal(SIGQUIT, ...)
*/
void	ctl_back_handler(int sig)
{
	(void)sig;
	write(2, "Quit (core dumped)\n", 18);
	g_sig_offset = 131;

	signal(SIGQUIT, ctl_back_handler);
	exit(131);
}

int	get_signal_interactive(void)
{
	signal(SIGINT, ctl_c_handler_interactive);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

int	get_signal_exec(void)
{
	ft_printf("starting exec signal handler\n");
	signal(SIGINT, ctl_c_handler_exec);
	signal(SIGQUIT, ctl_back_handler);
	return (0);
}

int	get_signal_heredoc(void)
{
    signal(SIGINT, ctl_c_handler_heredoc); // a function you define that breaks out of heredoc
    signal(SIGQUIT, ctl_back_handler);
	return (0);
}

int ignore_parent_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
