/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:04:43 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/20 02:19:16 by jbaumfal         ###   ########.fr       */
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

typedef enum	s_exec_error
{
	EXEC_ERR_FATAL,
	EXEC_ERR_NON_FATAL,
	EXEC_ERR_ACESS,
	EXEC_ERR_FILE,
	EXEC_SUCCESS,
	EXEC_NOT_FOUND
}	t_exec_error;

t_exec_error	builtin_parent(t_ast_node *node, t_shell *shell);
t_exec_error	builtin(t_ast_node *node, t_shell *shell);
t_exec_error	ft_echo(char **args, int argc);
t_exec_error	ft_pwd(t_directory *dir);
t_exec_error	ft_export(char **args, int argc, char ***envp);
t_exec_error	ft_unset(char **args, int argc, char **envp);
t_exec_error	ft_cd(char **args, int argc, t_directory *dir, char **envp);
t_exec_error	ft_env(char **envp, int argc);
t_exec_error	ft_exit(char *arg);
void		free_all(char **arg);
#endif
