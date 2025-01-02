/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:25:51 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/02 20:12:23 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

# define MAX_PID 32768

# define MAX_PIPE_COUNT 256

# define MAX_PROCESS_COUNT (MAX_PIPE_COUNT + 1)

typedef enum	s_exec_error
{
	EXEC_ERR_FATAL,
	EXEC_ERR_NON_FATAL,
	EXEC_ERR_ACESS,
	EXEC_ERR_FILE,
	EXEC_SUCCESS,
	EXEC_NOT_FOUND,
	EXEC_ERR_PIPE
}	t_exec_error;

typedef enum s_command_redir
{
	PIPE_T,
	FILE_T,
	STD_T
}	t_command_redir;

typedef struct s_exec_dat
{
	//REDIRECTION
	int					in_file;
	int					out_file;
	//PIPE
	t_command_redir	in_type;
	t_command_redir	out_type;
	//PROCCESS
	pid_t				child_pid;
	int					*status;
}	t_exec_data;



typedef struct s_shell		t_shell;
typedef struct s_ast_node	t_ast_node;

t_exec_error			start_command(t_shell *shell, t_ast_node *node);
t_exec_error			start_exec(t_shell *shell, t_ast_node *node);

void					exec_command(t_shell *shell, t_ast_node *node);

t_exec_error			set_input_output(t_shell *shell, t_ast_node *node);

void					exit_exec_status(t_exec_error	status);
void					link_pipe(t_ast_node	*node);

t_exec_error			start_pipeline(t_shell *shell, t_ast_node *node);

#endif
