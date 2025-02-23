/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@42.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 18:59:51 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/16 05:24:57 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	In a pipeline the command is executed a bit differently 
	as we can not launch the builtins in the parents anymore.

	We can therefore start directly with a child process
*/
t_exec_error	start_command_pipe(t_shell *shell, t_ast_node *node)
{
	pid_t			child_pid;
	t_exec_error	status;

	status = prepare_command(shell, node);
	if (status != EXEC_SUCCESS)
	{
		close_used_pipes(shell, node);
		return (status);
	}
	get_signal_exec();
	child_pid = fork();
	if (child_pid == -1)
		return (perror("total error: fork:"), EXEC_ERR_FATAL);
	shell->pid[shell->process_index++] = child_pid;
	if (child_pid == 0)
	{
		set_pipes(node, shell);
		exec_command(shell, node);
	}
	else
	{
		close_redirections(node);
		close_used_pipes(shell, node);
	}
	return (EXEC_SUCCESS);
}

t_exec_error	exec_pipeline(t_shell *shell, t_ast_node *node)
{
	t_exec_error	status;

	if (node->type == NODE_PIPE)
	{
		status = exec_pipeline(shell, node->u_data.s_pipe.left);
		if (status == EXEC_ERR_FATAL)
			return (status);
		status = exec_pipeline(shell, node->u_data.s_pipe.right);
		if (status == EXEC_ERR_FATAL)
			return (status);
	}
	if (node->type == NODE_COMMAND)
	{
		status = start_command_pipe(shell, node);
		if (status == EXEC_ERR_FATAL)
			return (status);
	}
	return (status);
}

/*
	This is the function to initialize the pipeline
		- The function is recursive and goes through the whole pipeline
		- For every pipe node the pipe is set with the pipe function
		- then we use the link_pipe function to link the pipes
*/

t_exec_error	init_pipeline(t_shell *shell, t_ast_node *node)
{
	t_exec_error	status;

	status = EXEC_SUCCESS;
	if (pipe(shell->pipes[shell->pipe_index]) == -1)
		return (EXEC_ERR_PIPE);
	link_pipe(node, shell);
	shell->pipe_index++;
	if (node->u_data.s_pipe.left->type == NODE_PIPE)
	{
		status = init_pipeline(shell, node->u_data.s_pipe.left);
		if (status == EXEC_ERR_PIPE)
			return (status);
	}	
	if (node->u_data.s_pipe.right->type == NODE_PIPE)
	{
		status = init_pipeline(shell, node->u_data.s_pipe.right);
		if (status == EXEC_ERR_PIPE)
			return (status);
	}
	return (status);
}

int	count_pipes(t_ast_node *node)
{
	int			counter;
	t_ast_node	*cursor;

	counter = 0;
	cursor = &*node;
	while (cursor->type == NODE_PIPE)
	{
		counter++;
		cursor = &*cursor->u_data.s_pipe.left;
	}
	return (counter);
}

/*
	Here we start the pipeline
		- we first launch the initialization of the pipeline 
		- then we start the execution of the pipeline
*/

t_exec_error	start_pipeline(t_shell *shell, t_ast_node *node)
{
	t_exec_error	status;

	shell->pipeline = true;
	status = init_pipeline(shell, node);
	if (status != EXEC_SUCCESS)
		return (status);
	status = exec_pipeline(shell, node);
	return (status);
}
