/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_in_out_files.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 01:08:11 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/05 15:48:56 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_infile(t_ast_node	*node, t_redir *redir, t_shell *shell)
{
	int	in_file;

	if (node->data.command.exec_data.in_type == PIPE_T)
		close(shell->pipes[node->data.command.exec_data.pipe_index_in][0]);
	in_file = open(redir->file, O_RDONLY);
	if (in_file == -1)
		return (perror("total error:  input file"), EXEC_ERR_FILE);
	if (node->data.command.exec_data.in_file != STDIN_FILENO)
		close(node->data.command.exec_data.in_file);
	node->data.command.exec_data.in_type = FILE_T;
	node->data.command.exec_data.in_file = in_file;
	return (EXEC_SUCCESS);
}

int	open_outfile(t_ast_node	*node, t_redir *redir, t_shell *shell)
{
	int	out_file;

	if (node->data.command.exec_data.out_type == PIPE_T)
		close(shell->pipes[node->data.command.exec_data.pipe_index_out][1]);
	if (redir->type == REDIR_APPEND)
		out_file = open(redir->file, O_CREAT | O_RDWR | O_APPEND, 0644);
	else
		out_file = open(redir->file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (out_file == -1)
		return (perror("total error: output file"), EXEC_ERR_FILE);
	node->data.command.exec_data.out_type = FILE_T;
	node->data.command.exec_data.out_file = out_file;
	return (EXEC_SUCCESS);
}

void	set_pipes(t_ast_node *node, t_shell *shell)
{
	t_exec_data	*data;

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
		if (dup2(shell->pipes[data->pipe_index_out][1], STDOUT_FILENO) == -1)
		{
			perror("dup2 for Pipe_OUT failed");
			exit(1);
		}
		close(shell->pipes[data->pipe_index_out][1]);
		close(shell->pipes[data->pipe_index_out][0]);
	}
}

/*
	in this function i now set all redirections liked to a command node
	
	as there can be multiple ones i do this in a while loop
	as soon as there is an error the loop stops and the folowing 
	redirections arent considered
	
*/
t_exec_error	set_infile_outfile(t_shell *shell, t_ast_node *node)
{
	t_exec_error	status;
	t_redir			*redir;

	redir = &*node->redirections;
	(void)shell;
	status = EXEC_SUCCESS;
	if (node->data.command.exec_data.in_type == STD_T)
		node->data.command.exec_data.in_file = STDIN_FILENO;
	if (node->data.command.exec_data.out_type == STD_T)
		node->data.command.exec_data.out_file = STDOUT_FILENO;
	while (redir)
	{
		if (redir->type == REDIR_INPUT || redir->type == REDIR_HEREDOC)
			status = open_infile(node, redir, shell);
		else if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND)
			status = open_outfile(node, redir, shell);
		if (status == EXEC_ERR_FILE)
			return (status);
		redir = &*redir->next;
	}
	return (status);
}
