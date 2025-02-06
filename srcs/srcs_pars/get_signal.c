/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_signal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 11:57:23 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/06 00:46:39 by kuru             ###   ########.fr       */
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

int	ignore_parent_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
