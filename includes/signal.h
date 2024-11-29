/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 11:26:37 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/11/29 13:53:34 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_H
# define SIGNAL_H
# include "minishell.h"
// extern int	g_signal = 0;

int get_signal();
int ctl_c_handler(int sig);
int ctl_d_handler(int sig);
int ctl_backslash_handler(int sig);


#endif