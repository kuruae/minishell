/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 18:59:51 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/20 01:56:46 by jbaumfal         ###   ########.fr       */
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
t_exec_error  start_command_pipe(t_shell *shell, t_ast_node *node)
{
	pid_t			child_pid;

	// if (node->data.command.exec_data.out_type != PIPE_T)
	// {
	// 	status = builtin(node, shell);
	// 	if (status != EXEC_NOT_FOUND)
	// 		return (status);
	// }
	child_pid = fork();
	if (child_pid == -1)
		return (perror("total error: fork:"), EXEC_ERR_FATAL);
	shell->pid[shell->process_index++] = child_pid;
	if (child_pid == 0)
	{
		exec_command(shell, node);
		ft_printf("Child process did not exit properly\n");
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
// t_exec_error	exec_command_pipe(t_shell *shell, t_ast_node *node)
// {
// 	int	index;

// 	ft_printf("Exec_command_pipe functions started\n");
// 	index = shell->process_index;
// 	ft_printf("index initialized to %d\n", index);
// 	shell->pid[index] = fork();
// 	if (shell->pid[index]  == -1)
// 		return (perror("total error: fork:"), EXEC_ERR_FATAL);
// 	if (shell->pid[index]== 0)
// 	{
// 		//ft_printf("Child process started\n");
// 		exec_command(shell, node);
// 		//normally it should not reach here
// 		//ft_printf("Child process did not exit properly\n");
// 		exit(1);
// 	}
// 	return (EXEC_SUCCESS);
// }

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
		// 	 return (status);
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

	//ft_printf("initializing pipe\n");
	status = EXEC_SUCCESS;
	if (pipe(shell->pipes[shell->pipe_index]) == -1)
		return (EXEC_ERR_PIPE);
	link_pipe(node, shell); //this function sets all the data in the commands that use this pipe
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
	int				i;

	i = 0;
	shell->pipeline = true;
	status = init_pipeline(shell, node);
	if (status != EXEC_SUCCESS)
		return (status);
	status = exec_pipeline(shell, node);
	//here i close all the pipes (this part should onl be reached by the parrent)
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