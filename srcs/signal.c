/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 11:57:23 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/11/29 13:49:50 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

# include <signal.h>

int get_signal();
int  ctl_c_handler(int sig);
int  ctl_d_handler(int sig);
int  ctl_backslash_handler(int sig);


int get_signal()
{
    signal(SIGINT, ctl_c_handler);
}

int  ctl_c_handler(int sig)
{}

int  ctl_d_handler(int sig)
{
    ft_printf("Received Signal + C\n");
}

int  ctl_backslash_handler(int sig)
{}


