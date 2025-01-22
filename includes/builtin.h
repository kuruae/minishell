/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:04:43 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/02 15:19:14 by jbaumfal         ###   ########.fr       */
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

t_exec_error	builtin(t_ast_node *node, t_shell *shell);
t_exec_error	ft_echo(char **args, int argc);
t_exec_error	ft_pwd(t_directory *dir);
t_exec_error	ft_export(char **args, int argc, char ***envp);
t_exec_error	ft_unset(char **args, int argc, char **envp);
t_exec_error	ft_cd(char **args, int argc, t_directory *dir, char ***envp);
t_exec_error	ft_env(char **envp, int argc);
t_exec_error	ft_exit(char *arg);
void		free_all(char **arg);
#endif
