/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_in_out_files.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 01:08:11 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/06 21:17:17 by jbaumfal         ###   ########.fr       */
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

t_exec_error	appending_heredocs(t_redir *redir, int main_heredoc)
{
	int		extra_heredoc;
	char	*line;

	while (redir->next && redir->next->type == REDIR_HEREDOC)
	{
		redir = &*redir->next;
		extra_heredoc = open(redir->file, O_RDONLY, 0644);
		line = get_next_line(extra_heredoc);
		while (line)
		{
			ft_putstr_fd(line, main_heredoc);
			free(line);
			line = NULL;
			line = get_next_line(extra_heredoc);
		}
		close(extra_heredoc);
		line = NULL;
	}
	return (EXEC_SUCCESS);
}

int	open_heredocs(t_ast_node *node, t_redir *redir, t_shell *shell)
{
	int				main_heredoc;
	t_redir			*first;
	t_exec_error	status;

	if (node->data.command.exec_data.in_type == PIPE_T)
		close(shell->pipes[node->data.command.exec_data.pipe_index_in][0]);
	main_heredoc = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (main_heredoc == -1)
		return (perror("total error: heredoc file"), EXEC_ERR_FILE);
	first = &*redir;
	status = appending_heredocs(redir, main_heredoc);
	close (main_heredoc);
	node->data.command.exec_data.in_type = FILE_T;
	node->data.command.exec_data.in_file = open(first->file, O_RDONLY, 0644);
	if (node->data.command.exec_data.in_file == -1)
		return (perror("total error: heredoc file"), EXEC_ERR_FILE);
	return (EXEC_SUCCESS);
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
	status = EXEC_SUCCESS;
	if (node->data.command.exec_data.in_type == STD_T)
		node->data.command.exec_data.in_file = STDIN_FILENO;
	if (node->data.command.exec_data.out_type == STD_T)
		node->data.command.exec_data.out_file = STDOUT_FILENO;
	while (redir)
	{
		if (redir->type == REDIR_INPUT)
			status = open_infile(node, redir, shell);
		else if (redir->type == REDIR_HEREDOC)
		{
			status = open_heredocs(node, redir, shell);
			while (redir->next && redir->next->type == REDIR_HEREDOC)
				redir = &*redir->next;
		}
		else if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND)
			status = open_outfile(node, redir, shell);
		redir = &*redir->next;
	}
	return (status);
}
