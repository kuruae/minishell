/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_in_out_files.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 01:08:11 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/18 01:42:37 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_infile(t_ast_node	*node)
{
	int	in_file;

	in_file = open(node->redirections->file, O_RDONLY);
	if (in_file == -1)
		return (perror("total error:  input file"), EXEC_ERR_FILE);
	node->data.command.exec_data.in_redir = true;
	node->data.command.exec_data.in_file = in_file;
	ft_printf("fd_in: %d\n", in_file);
	return (EXEC_SUCCESS);
}

int	open_outfile(t_ast_node	*node)
{
	int	out_file;

	out_file = open(node->redirections->file, O_CREAT | O_RDWR | O_TRUNC, 644);
	if (out_file == -1)
		return (perror("total error: output file"), EXEC_ERR_FILE);
	node->data.command.exec_data.out_redir = true;
	node->data.command.exec_data.out_file = out_file;
	return (EXEC_SUCCESS);
}

t_exec_error	set_input_output(t_shell *shell, t_ast_node *node)
{
	t_exec_error	status;

	(void)shell;
	//setting all values concerning redirection to the standard
	node->data.command.exec_data.in_file = STDIN_FILENO;
	node->data.command.exec_data.out_file = STDOUT_FILENO;
	//the following bools show if there is a redirection or not
	node->data.command.exec_data.in_redir = false;
	node->data.command.exec_data.out_redir = false;
	if (!(node->redirections))
		return (EXEC_SUCCESS);
	else if (node->redirections->type == REDIR_INPUT)
		status = open_infile(node);
	else if (node->redirections->type == REDIR_OUTPUT)
		status = open_outfile(node);
	if (status == EXEC_ERR_FILE)
		return (status);
	return (EXEC_SUCCESS);
}