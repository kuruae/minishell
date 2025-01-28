/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_signal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 11:57:23 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/28 19:21:08 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	ctl_c_handler_interactive(int sig)
{
	//ft_printf("handeling ctl c as interactive\n");
	(void)sig;
	ft_printf("\n");
	g_sig_offset = 130;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	signal(SIGINT, ctl_c_handler_interactive);
}

void	ctl_c_handler_exec(int sig)
{
	//ft_printf("handeling ctl c as exec\n");
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	signal(SIGINT, ctl_c_handler_exec);
	exit(130);
	// rl_on_new_line();
    // rl_replace_line("", 0);
    // rl_redisplay();
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

void ctl_c_handler_heredoc(int sig)
{
    (void)sig;
    write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
    exit(130);
}

int	get_signal_interactive(void)
{
	signal(SIGINT, ctl_c_handler_interactive);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

int	get_signal_exec(void)
{
	//ft_printf("starting exec signal handler\n");
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
