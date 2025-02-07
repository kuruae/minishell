/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:04:43 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/07 20:38:08 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"

# define MAX_PATH 4096

typedef struct s_ast_node	t_ast_node;
typedef struct s_shell		t_shell;

typedef struct s_directory
{
	char	current_path[MAX_PATH];
	char	old_path[MAX_PATH];
	char	home_path[MAX_PATH];
}	t_directory;

t_exec_error	builtin(t_ast_node *node, t_shell *shell, int fd_out);
t_exec_error	ft_echo(char **args, int argc, int fd_out);
t_exec_error	ft_pwd(t_directory *dir, int fd_out);
t_exec_error	ft_export(char **args, int argc, char ***envp);
t_exec_error	ft_unset(char **args, int argc, char **envp);
t_exec_error	ft_cd(char **args, int argc, t_directory *dir, t_shell *shell);
t_exec_error	ft_env(char **envp, int argc, int fd_out);
t_exec_error	ft_exit(char **args, int argc, t_shell *shell);
void			free_all(char **arg);
void			set_sig_offset(t_exec_error status);
#endif
