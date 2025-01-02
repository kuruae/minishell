/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 18:59:51 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/02 22:33:26 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exec_error	exec_command_pipe(t_shell *shell, t_ast_node *node)
{
	(void)shell;
	(void)node;
	return (EXEC_SUCCESS);
}

t_exec_error	exec_pipeline(t_shell *shell, t_ast_node *node)
{
	if (node->type == NODE_PIPE)
		exec_pipeline(shell, node->data.pipe.left);
	if (node->data.pipe.right->type == NODE_COMMAND)
		exec_command_pipe(shell, node);
	if (node->type == NODE_COMMAND)	
		exec_command_pipe(shell, node);
	return (EXEC_SUCCESS);
}

t_exec_error	init_pipeline(t_shell *shell, t_ast_node *node)
{
	t_exec_error	status;

	ft_printf("initializing pipe\n");
	status = EXEC_SUCCESS;
	if (node->data.pipe.left->type == NODE_PIPE)
	{
		status = init_pipeline(shell, node->data.pipe.left) == EXEC_ERR_PIPE;
		if (status == EXEC_ERR_PIPE)
			return (status);
	}	
	if (node->data.pipe.right->type == NODE_PIPE)
	{
		status = init_pipeline(shell, node->data.pipe.right) == EXEC_ERR_PIPE;
		if (status == EXEC_ERR_PIPE)
			return (status);
	}
	if (pipe(node->data.pipe.pipe) == -1)
	{
		status = EXEC_ERR_PIPE;
		link_pipe(node);
	}
	return (status);
}

t_exec_error	start_pipeline(t_shell *shell, t_ast_node *node)
{
	t_exec_error	status;

	status = init_pipeline(shell, node);
	if (status != EXEC_SUCCESS)
		return (status);
	status = exec_pipeline(shell, node);
	return (status);
}

// t_exec_error	start_pipe(t_shell *shell, t_ast_node *node)
// {
// 	t_ast_node		*command_in; //command left of the pipe
// 	t_ast_node		*command_out; //command right of the pipe
// 	t_exec_error	status;

// 	(void)shell;
// 	if (pipe(node->data.pipe.pipe) == -1)
// 		return (perror("total error: pipe"), ERR_FATAL);
// 	command_in = NULL;
// 	command_out = NULL;
// 	if (node->data.pipe.left->type == NODE_COMMAND)
// 		command_in = node->data.pipe.left;
// 	if (node->data.pipe.right->type == NODE_COMMAND)
// 		command_out = node->data.pipe.right;
// 	if (!command_in || !command_in)
// 		ft_putstr_fd("total error: missing command before or after pipe\n", 2);
// 	command_in->data.command.exec_data.out_redir = true;
// 	command_in->data.command.exec_data.out_file = node->data.pipe.pipe[0];
// 	command_in->data.command.exec_data.in_redir = true;
// 	command_in->data.command.exec_data.in_file = node->data.pipe.pipe[1];
// 	status = start_command(shell, command_in);
// 	if (status == EXEC_ERR_FATAL)
// 		return (EXEC_ERR_FATAL);
// 	status = start_command(shell, command_out);
// 	if (status == EXEC_ERR_FATAL)
// 		return (EXEC_ERR_FATAL);
// 	close(node->data.pipe.pipe[0]);
// 	close(node->data.pipe.pipe[1]);
// 	return (status);
// }