/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:25:51 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/14 19:42:22 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

typedef struct s_shell t_shell;
typedef struct s_ast_node t_ast_node;

typedef enum	s_exec_error
{
	EXEC_ERR_FATAL,
	EXEC_ERR_NON_FATAL,
	EXEC_ERR_OUTFILE,
	EXEC_SUCCESS,
}	t_exec_error;

t_exec_error start_exec(t_shell *shell, t_ast_node *node);

t_exec_error exec_command(t_shell *shell, t_ast_node *node, int fd_out);

#endif
