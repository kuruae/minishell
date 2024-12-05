/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:04:43 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/05 16:58:37 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"

# define MAX_PATH 4096

typedef struct s_directory
{
	char	current_path[MAX_PATH];
	char	old_path[MAX_PATH];
}	t_directory;

int		builtin(char *command, char *arg, int fd_out);
int		ft_echo(char *string, int fd_out);
int		ft_pwd(t_directory *dir, int fd_out);
int		ft_cd(char *path, t_directory *dir);
int	ft_exit(char *arg);
#endif
