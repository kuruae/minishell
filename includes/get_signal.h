/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_signal.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:55:12 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/07 23:49:15 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_SIGNAL_H
# define GET_SIGNAL_H

# include "types.h"
# include "minishell.h"

int		get_signal_interactive(void);
int		get_signal_exec(void);
void	ctl_c_handler_interactive(int sig);
void	ctl_c_handler_exec(int sig);
void	ctl_d_handler(int sig);
void	ctl_c_handler_heredoc(int sig);
void	ctl_back_handler(int sig);
int		get_signal_heredoc(void);
int		ignore_parent_signals(void);
#endif
