/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_signal.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:55:12 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/02 15:35:13 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_SIGNAL_H
# define GET_SIGNAL_H
# include <features.h>
// # ifndef _POSIX_C_SOURCE
// #  define _POSIX_C_SOURCE 199309L
// # endif
# include <signal.h>

int		get_signal(void);
void	ctl_c_handler(int sig);
void	ctl_d_handler(int sig);
// int ctl_backslash_handler(int sig);
#endif