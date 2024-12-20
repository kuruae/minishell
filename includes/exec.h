/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:25:51 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/19 23:57:59 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

# define MAX_PID 32768

typedef struct s_exec_data
{
	//REDIRECTION
	bool	in_redir;
	int		in_file;
	bool	out_redir;
	int		out_file;
	//PROCCESS
	pid_t	child_pid;
	int		*status;
}	t_exec_data;

typedef struct s_shell		t_shell;
typedef struct s_ast_node	t_ast_node;

t_exec_error			start_exec(t_shell *shell, t_ast_node *node);

void					exec_command(t_shell *shell, t_ast_node *node);

t_exec_error			set_input_output(t_shell *shell, t_ast_node *node);

void					exit_exec_status(t_exec_error	status);

#endif
