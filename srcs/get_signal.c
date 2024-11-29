/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_signal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 11:57:23 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/11/29 18:54:17 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int     get_signal();
void    ctl_c_handler(int sig);
void    ctl_d_handler(int sig);
// int  ctl_backslash_handler(int sig);


int get_signal()
{
    struct sigaction	sa;

	sa.sa_handler = ctl_c_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
        perror("sigaction");
        return (1);
	}
    // signal(SIGINT, ctl_c_handler);
    // signal(SIGQUIT, ctl_d_handler);
    // signal(SIGINT, ctl_c_handler);

    return (0);
}

void  ctl_c_handler(int sig)
{
    (void)sig;
    ft_printf("Received Signal + C\n");
}

void  ctl_d_handler(int sig)
{
    (void)sig;
    ft_printf("Received Signal + D\n");
}

// int  ctl_backslash_handler(int sig)
// {
//     (void)sig;
//     ft_printf("Received Signal + \\ \n");
//     return (0);
// }


