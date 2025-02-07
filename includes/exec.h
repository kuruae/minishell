/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:25:51 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/07 23:51:17 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "types.h"
# include "minishell.h"

# define MAX_PID 32768

# define MAX_PIPE_COUNT 256

# define MAX_PROCESS_COUNT 257

# define READ_END 0

# define WRITE_END 1

typedef enum s_command_redir
{
	STD_T,
	PIPE_T,
	FILE_T
}	t_command_redir;

typedef struct s_exec_dat
{
	int				in_file;
	int				out_file;
	t_command_redir	in_type;
	t_command_redir	out_type;
	int				pipe_index_in;
	int				pipe_index_out;
	int				*status;
}	t_exec_data;



typedef struct s_shell		t_shell;
typedef struct s_ast_node	t_ast_node;

t_exec_error			start_command(t_shell *shell, t_ast_node *node);
t_exec_error			start_exec(t_shell *shell, t_ast_node *node);

t_exec_error			prepare_command_exec(t_shell *shell, t_ast_node *node);

void					exec_command(t_shell *shell, t_ast_node *node);

t_exec_error			set_infile_outfile(t_shell *shell, t_ast_node *node);
void					set_pipes(t_ast_node *node, t_shell *shell);

void					exit_exec_status(t_exec_error	status, t_shell *shell);
void					link_pipe(t_ast_node *node, t_shell *shell);
void					close_used_fds(t_shell *shell, t_ast_node *node);
void					close_unused_pipes(t_ast_node *node, t_shell *shell);
t_shell					init_subshell(t_shell	*shell, t_ast_node *node);

t_exec_error			start_pipeline(t_shell *shell, t_ast_node *node);
int						count_pipes(t_ast_node *node);

void					close_used_pipes(t_shell *shell, t_ast_node *node);
bool					is_directory(char *path);
t_exec_error			return_exit_status(int g_sig_offset);
void					analize_child_status(int child_status);
t_exec_error			prepare_command(t_shell *shell, t_ast_node *node);

char					**get_paths(char **env);
#endif
