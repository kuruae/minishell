/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:29:09 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/17 12:31:45 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


int	open_outfile(t_redir *redir)
{
	int file_out;

	file_out = open(redir->file, O_CREAT | O_RDWR | O_TRUNC, 644);
	redir->fd_redir = file_out;
	return (file_out);
}

t_exec_error start_command(t_shell *shell, t_ast_node *node)
{
	int	fd_out;

	if (!(node->redirections))
		fd_out = STDOUT_FILENO;
	else
		fd_out = open_outfile(node->redirections);
	ft_printf("fd_out: %d\n", fd_out);
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
		if (status == EXEC_ERR_FATAL)
			return (EXEC_ERR_FATAL);
	}
	return (EXEC_SUCCESS);
}

