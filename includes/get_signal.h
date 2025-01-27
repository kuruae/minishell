/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_signal.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:55:12 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/27 00:24:37 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_SIGNAL_H
# define GET_SIGNAL_H

# include "minishell.h"

int		get_signal_interactive(void);
int		get_signal_exec(void);
void	ctl_c_handler_interactive(int sig);
void	ctl_c_handler_exec(int sig);
void	ctl_d_handler(int sig);
void	ctl_back_handler(int sig);
void	get_signal_heredoc(void);
#endif
