/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_signal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 11:57:23 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/01 19:35:05 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int     get_signal();
void    ctl_c_handler(int sig);
void    ctl_d_handler(int sig);
// int  ctl_backslash_handler(int sig);


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
	ft_printf("Received Signal ctrl + C\n");
	// g_exit_status = 130;
	rl_on_new_line();
	rl_redisplay();
	signal(SIGINT, ctl_c_handler);
}

void  ctl_d_handler(int sig)
{
	(void)sig;
	ft_printf("Received Signal + D\n");
	rl_on_new_line();
}



