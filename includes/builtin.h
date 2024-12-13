/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:04:43 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/13 21:34:51 by jbaumfal         ###   ########.fr       */
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
	char	home_path[MAX_PATH];
}	t_directory;

typedef enum	s_bi_error
{
	BI_ERR_NO_BUILT_IN,
	BI_ERR_MALLOC,
	BI_SUCCESS,
	BI_ERR_NON_FATAL
}	t_bi_error;

t_bi_error	builtin(char *command, char *arg, int fd_out, char ***envp);
void		free_all(char **arg);
t_bi_error	ft_echo(char *string, int fd_out);
t_bi_error	ft_pwd(t_directory *dir, int fd_out);
t_bi_error	ft_cd(char *path, t_directory *dir, char **env);
t_bi_error	ft_env(char **envp, int fd_out);
t_bi_error	ft_exit(char *arg);
t_bi_error	ft_unset(char *arg,char **envp);
t_bi_error	ft_export(char *arg,char ***envp);
#endif
