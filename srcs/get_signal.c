/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_signal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 11:57:23 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/02 16:04:57 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     get_signal();
void    ctl_c_handler(int sig);
void    ctl_back_handler(int sig);

int get_signal()
{
	// struct sigaction	sa;

	// sa.sa_sigaction = ctl_c_handler;
	// sigemptyset(&sa.sa_mask);
	// sa.sa_flags = SA_RESTART;
	// if (sigaction(SIGINT, &sa, NULL) == -1)
	// {
    //     perror("sigaction");
    //     return (1);
	// }
	signal(SIGINT, ctl_c_handler);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

void  ctl_c_handler(int sig)
{

	(void)sig;
	rl_redisplay();
	ft_printf("\n");
	g_sig_offset = 130;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	signal(SIGINT, ctl_c_handler);
}

void  ctl_back_handler(int sig)
{
	// currently we dont use this function as we can ignore the signal directly in the get_signal function
	// if we need to change the g_sig_offset value to 131 we can use this function and insert it in signal(SIGQUIT, ...)
	(void)sig;
	//ft_printf("Ignored SignalQuit\n");
	g_sig_offset = 131;
	//rl_on_new_line();
	//rl_redisplay();
	signal(SIGQUIT, SIG_IGN);
	signal(SIGQUIT, ctl_back_handler);
}



