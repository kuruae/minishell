/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_in_out_files.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 01:08:11 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/25 18:12:46 by jbaumfal         ###   ########.fr       */
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
	return (EXEC_SUCCESS);
}

int	open_outfile(t_ast_node	*node, t_redir *redir)
{
	int	out_file;
	
	if (redir->type == REDIR_APPEND)
		out_file = open(redir->file, O_CREAT | O_RDWR | O_APPEND, 0644);
	else
		out_file = open(redir->file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (out_file == -1)
		return (perror("total error: output file"), EXEC_ERR_FILE);
	node->data.command.exec_data.out_type = FILE_T;
	node->data.command.exec_data.out_file = out_file;
	//ft_printf("fd_out: %d\n", out_file);
	return (EXEC_SUCCESS);
}



void set_pipes(t_ast_node *node, t_shell *shell)
{
	t_exec_data *data;

	data = &node->data.command.exec_data;
	if (data->in_type == PIPE_T)
	{
		if (dup2(shell->pipes[data->pipe_index_in][0], STDIN_FILENO) == -1)
		{
			perror("dup2 for Pipe_IN failed");
			exit(1);
		}
		close(shell->pipes[data->pipe_index_in][0]);
		close(shell->pipes[data->pipe_index_in][1]);
	}
	if (data->out_type == PIPE_T)
	{
		//ft_printf("dup2 for Pipe_OUT command: %s\n", node->data.command.command);
		if (dup2(shell->pipes[data->pipe_index_out][1], STDOUT_FILENO) == -1)
		{
	 		perror("dup2 for Pipe_OUT failed");
			exit(1);
		}
		close(shell->pipes[data->pipe_index_out][1]);
		close(shell->pipes[data->pipe_index_out][0]);
	}
}

t_exec_error	set_infile_outfile(t_shell *shell, t_ast_node *node)
{
	t_exec_error	status;

	(void)shell;
	//first i set all data
	status = EXEC_SUCCESS;
	if (node->data.command.exec_data.in_type == STD_T)
		node->data.command.exec_data.in_file = STDIN_FILENO;
	if (node->data.command.exec_data.out_type == STD_T)
		node->data.command.exec_data.out_file = STDOUT_FILENO;
	if (node->redirections && (node->redirections->type == REDIR_INPUT || node->redirections->type == REDIR_HEREDOC))
	{
		status = open_infile(node);
		if (status == EXEC_ERR_FILE)
			return (status);
		if (node->redirections->next && (node->redirections->next->type == REDIR_OUTPUT || node->redirections->next->type == REDIR_APPEND))
			status = open_outfile(node, node->redirections->next);
	}
	if (node->redirections && (node->redirections->type == REDIR_OUTPUT || node->redirections->type == REDIR_APPEND))
		status = open_outfile(node, node->redirections);
	if (status == EXEC_ERR_FILE)
		return (status);
	return (EXEC_SUCCESS);
}
