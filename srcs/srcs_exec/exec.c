/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:29:09 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/15 01:29:10 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


int	open_outfile(t_redir *redir)
{
	int file_out;

	file_out = open(redir->file, O_CREAT | O_RDWR | O_TRUNC, 644);
	return (file_out);
}


t_exec_error start_command(t_shell *shell, t_ast_node *node)
{
	int	fd_out;

	if (!(node->redirections))
		fd_out = STDOUT_FILENO;
	else
		fd_out = open_outfile(node->redirections);
	if (fd_out == -1)
		return (perror("total erro: opening file") ,EXEC_ERR_OUTFILE);
	return (exec_command(shell, node, fd_out));
}

t_exec_error start_exec(t_shell *shell, t_ast_node *node)
{
	t_exec_error status;
	(void)shell;
	if (node->type == NODE_COMMAND)
	{
		status = start_command(shell, node);
	}
	(void)status;
	return (EXEC_SUCCESS);
}

