/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_signal.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:55:12 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/13 23:41:51 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_SIGNAL_H
# define GET_SIGNAL_H

# include "minishell.h"

int		get_signal(void);
void	ctl_c_handler(int sig);
void	ctl_d_handler(int sig);
// int ctl_backslash_handler(int sig);
#endif
