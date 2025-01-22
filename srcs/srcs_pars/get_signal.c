/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_signal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 11:57:23 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/20 15:05:12 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_signal_interactive(void)
{
	signal(SIGINT, ctl_c_handler_interactive);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

int	get_signal_exec(void)
{
	signal(SIGQUIT, ctl_back_handler);
	signal(SIGINT, ctl_c_handler_exec);
	return (0);
}


void	ctl_c_handler_interactive(int sig)
{
	(void)sig;
	rl_redisplay();
	ft_printf("\n");
	g_sig_offset = 130;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	signal(SIGINT, ctl_c_handler_interactive);
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

/**
 * currently we dont use this function as
 *  we can ignore the signal directly in the get_signal function
 * if we need to change the g_sig_offset value to 131,
 * 	we can use this function and insert it in signal(SIGQUIT, ...)
*/
void	ctl_back_handler(int sig)
{
	(void)sig;
	g_sig_offset = 131;
	signal(SIGQUIT, ctl_back_handler);
	exit(131);
}
