/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 18:59:51 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/05 15:48:35 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	In a pipeline the command is executed a bit differently 
	as we can not launch the builtins in the parents anymore.

	We can therefore start directly with a child process

	The only exeption is when the command is the last in the pipeline
	-> in this case the out_type wont be PIPE_T
	we therefore have to check if the command is a builtin
*/
t_exec_error	start_command_pipe(t_shell *shell, t_ast_node *node)
{
	pid_t			child_pid;
	t_exec_error	status;

	status = prepare_command(shell, node);
	if (status != EXEC_SUCCESS)
		return (status);
	get_signal_exec();
	child_pid = fork();
	if (child_pid == -1)
		return (perror("total error: fork:"), EXEC_ERR_FATAL);
	shell->pid[shell->process_index++] = child_pid;
	if (child_pid == 0)
	{
		set_pipes(node, shell);
		exec_command(shell, node);
		exit(1);
	}
	else
	{
		if (node->data.command.exec_data.in_type == PIPE_T)
			close(shell->pipes[node->data.command.exec_data.pipe_index_in][0]);
		if (node->data.command.exec_data.out_type == PIPE_T)
			close(shell->pipes[node->data.command.exec_data.pipe_index_out][1]);
	}
	return (EXEC_SUCCESS);
}

t_exec_error	exec_pipeline(t_shell *shell, t_ast_node *node)
{
	t_exec_error	status;

	if (node->type == NODE_PIPE)
	{
		status = exec_pipeline(shell, node->data.pipe.left);
		if (status == EXEC_ERR_FATAL)
			return (status);
		status = exec_pipeline(shell, node->data.pipe.right);
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

t_exec_error	init_pipeline(t_shell *shell, t_ast_node *node)
{
	t_exec_error	status;

	status = EXEC_SUCCESS;
	if (pipe(shell->pipes[shell->pipe_index]) == -1)
		return (EXEC_ERR_PIPE);
	link_pipe(node, shell);
	shell->pipe_index++;
	if (node->data.pipe.left->type == NODE_PIPE)
	{
		status = init_pipeline(shell, node->data.pipe.left);
		if (status == EXEC_ERR_PIPE)
			return (status);
	}	
	if (node->data.pipe.right->type == NODE_PIPE)
	{
		status = init_pipeline(shell, node->data.pipe.right);
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
		cursor = &*cursor->data.pipe.left;
	}
	return (counter);
}

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
