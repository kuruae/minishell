/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_signal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@42.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 11:57:23 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/22 14:04:17 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Here we set the signal handlers for the different situations

	We have 3 different situations in which the signals are handled differently:
		- Interactive mode (When the user can write something in the command line)
		- Execution mode (This is wjile commands are executed -> especially for the situation where the execution is not ending)
			Example: "cat" with no input
		- Heredoc mode
			The moment when the user is writing in the heredoc
	
*/

int	get_signal_interactive(void)
{
	signal(SIGINT, ctl_c_handler_interactive);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

int	get_signal_exec(void)
{
	signal(SIGINT, ctl_c_handler_exec);
	signal(SIGQUIT, ctl_back_handler);
	return (0);
}

int	get_signal_heredoc(void)
{
	signal(SIGINT, ctl_c_handler_heredoc);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
