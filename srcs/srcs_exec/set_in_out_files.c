/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_in_out_files.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 01:08:11 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/11 20:21:30 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_infile(t_ast_node	*node)
{
	int	in_file;

	//ft_printf("opening infile\n");

	//ft_printf("Setting new Infile to %s\n", node->redirections->file);
	in_file = open(node->redirections->file, O_RDONLY);
	if (in_file == -1)
		return (perror("total error:  input file"), EXEC_ERR_FILE);
	node->data.command.exec_data.in_type = FILE_T;
	node->data.command.exec_data.in_file = in_file;
	ft_printf("fd_in: %d\n", in_file);
	dup2(in_file, STDIN_FILENO);
	return (EXEC_SUCCESS);
}

int	open_outfile(t_ast_node	*node, bool second)
{
	int	out_file;

	//ft_printf("opening out file\n");

	//ft_printf("Setting new Out file to %s\n", node->redirections->file);
	if (second == true)
		out_file = open(node->redirections->next->file, O_CREAT | O_RDWR | O_TRUNC, 644);
	else
		out_file = open(node->redirections->file, O_CREAT | O_RDWR | O_TRUNC, 644);
	if (out_file == -1)
		return (perror("total error: output file"), EXEC_ERR_FILE);
	node->data.command.exec_data.out_type = FILE_T;
	node->data.command.exec_data.out_file = out_file;
	//ft_printf("fd_out: %d\n", out_file);
	dup2(out_file, STDOUT_FILENO);
	close(out_file);
	return (EXEC_SUCCESS);
}



void set_pipes(t_ast_node *node, t_shell *shell)
{
	t_exec_data *data;

	//ft_printf("starting set_pipes\n");
	data = &node->data.command.exec_data;
	if (data->in_type == PIPE_T)
	{
		//ft_printf("dup2 for Pipe_IN, command: %s\n", node->data.command.command);
		if (dup2(shell->pipes[data->pipe_index_in][0], STDIN_FILENO) == -1)
		{
			perror("dup2 for Pipe_IN failed");
			exit(1);
		}
	}
	if (data->out_type == PIPE_T)
	{
		//ft_printf("dup2 for Pipe_OUT command: %s\n", node->data.command.command);
		if (dup2(shell->pipes[data->pipe_index_out][1], STDOUT_FILENO) == -1)
		{
	 		perror("dup2 for Pipe_OUT failed");
			exit(1);
		}
		}
	close_unused_pipes(node, shell);
}

t_exec_error	set_input_output(t_shell *shell, t_ast_node *node)
{
	t_exec_error	status;

	(void)shell;
	//ft_printf("starting to set in and out\n");
	//first i set all data
	if (node->data.command.exec_data.in_type == STD_T)
		node->data.command.exec_data.in_file = STDIN_FILENO;
	if (node->data.command.exec_data.out_type == STD_T)
		node->data.command.exec_data.out_file = STDOUT_FILENO;
	if (node->redirections && node->redirections->type == REDIR_INPUT)
	{
		//ft_printf("opening infile\n");
		status = open_infile(node);
		if (status == EXEC_ERR_FILE)
			return (status);
		if (node->redirections->next && node->redirections->next->type == REDIR_OUTPUT)
			status = open_outfile(node, true);
	}
	if (node->redirections && node->redirections->type == REDIR_OUTPUT)
		status = open_outfile(node, false);
	if (status == EXEC_ERR_FILE)
		return (status);
	set_pipes(node, shell);
	return (EXEC_SUCCESS);
}
