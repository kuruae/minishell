/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_in_out_files.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 01:08:11 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/03 02:49:25 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_infile(t_ast_node	*node)
{
	int	in_file;

	ft_printf("Setting new Infile to %s\n", node->redirections->file);
	in_file = open(node->redirections->file, O_RDONLY);
	if (in_file == -1)
		return (perror("total error:  input file"), EXEC_ERR_FILE);
	node->data.command.exec_data.in_type = FILE_T;
	node->data.command.exec_data.in_file = in_file;
	ft_printf("fd_in: %d\n", in_file);
	dup2(in_file, STDIN_FILENO);
	close(in_file);
	return (EXEC_SUCCESS);
}

int	open_outfile(t_ast_node	*node, bool second)
{
	int	out_file;

	ft_printf("Setting new Out file to %s\n", node->redirections->file);
	if (second == true)
		out_file = open(node->redirections->next->file, O_CREAT | O_RDWR | O_TRUNC, 644);
	else
		out_file = open(node->redirections->file, O_CREAT | O_RDWR | O_TRUNC, 644);
	if (out_file == -1)
		return (perror("total error: output file"), EXEC_ERR_FILE);
	node->data.command.exec_data.out_type = FILE_T;
	node->data.command.exec_data.out_file = out_file;
	ft_printf("fd_out: %d\n", out_file);
	dup2(out_file, STDOUT_FILENO);
	close(out_file);
	return (EXEC_SUCCESS);
}

void			set_pipes(t_ast_node	*node)
{
	t_exec_data	*data;

	data = &*node->data.command.exec_data;
	if (data->in_type == PIPE_T)
	 dup2()
}

t_exec_error	set_input_output(t_shell *shell, t_ast_node *node)
{
	t_exec_error	status;

	(void)shell;
	if (node->data.command.exec_data.in_type == STD_T)
		node->data.command.exec_data.in_file = STDIN_FILENO;
	if (node->data.command.exec_data.out_type == STD_T)
		node->data.command.exec_data.out_file = STDIN_FILENO;
	//the following bools show if there is a redirection or not
	if (!(node->redirections))
		return (EXEC_SUCCESS);
	else if (node->redirections->type == REDIR_INPUT)
	{
		status = open_infile(node);
		if (status == EXEC_ERR_FILE)
			return (status);
		if (node->redirections->next->type == REDIR_OUTPUT)
			status = open_outfile(node, true);
	}
	if (node->redirections->type == REDIR_OUTPUT)
		status = open_outfile(node, false);
	if (status == EXEC_ERR_FILE)
		return (status);
	
	return (EXEC_SUCCESS);
}
