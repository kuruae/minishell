/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:25:51 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/17 16:31:42 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

# define MAX_PID 32768

typedef struct s_exec_data
{
	bool	in_redir;
	int		in_file;
	bool	out_redir;
	int		out_file;
}	t_exec_data;

typedef struct s_shell		t_shell;
typedef struct s_ast_node	t_ast_node;

t_exec_error	start_exec(t_shell *shell, t_ast_node *node);

t_exec_error	exec_command(t_shell *shell, t_ast_node *node, int fd_out);

#endif
