/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@42.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 23:45:35 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/22 14:12:00 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	This function is used in the child processes
		As we cant we exit return values of the functions from the child process in the parrent 
		we have to use the exit status.
	We try to use the exit status numbers just like in the real bash (exmple: NOT FOUND = 126)
		-> we will later be able to access the exit code with the waitpid function in the parrent process
*/

void	exit_exec_status(t_exec_error	status, t_shell *shell)
{
	clean_up_child(shell);
	if (status == EXEC_ERR_FATAL)
		exit(128);
	if (status == EXEC_SUCCESS)
		exit(0);
	if (status == EXEC_ERR_NON_FATAL)
		exit(1);
	if (status == EXEC_ERR_ACCESS)
		exit(126);
	if (status == EXEC_ERR_FILE)
		exit(1);
	if (status == EXEC_NOT_FOUND)
		exit(127);
	exit(1);
}

/*
	In the link pipe function we make sure that the new pipe we created is reachable / registered in the other parts of the AST tree that need them
		-> precisely those are the command nodes that are connected to the pipe node
	- First we the a pipeline with multiple pipes the node to the left of a pipe  can also be a pipe
		-> to get the previous command we will therefor look for the right node of that pipe to the left
*/

void	link_pipe(t_ast_node *node, t_shell *shell)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_ast_node	*last_command;

	left = node->u_data.s_pipe.left;
	right = node->u_data.s_pipe.right;
	if (left->type == NODE_COMMAND)
	{
		left->u_data.s_command.exec_data.out_type = PIPE_T;
		left->u_data.s_command.exec_data.pipe_index_out = shell->pipe_index;
	}
	if (right->type == NODE_COMMAND)
	{
		right->u_data.s_command.exec_data.in_type = PIPE_T;
		right->u_data.s_command.exec_data.pipe_index_in = shell->pipe_index;
	}
	if (left->type == NODE_PIPE)
	{
		last_command = left;
		if (last_command->type == NODE_PIPE)
			last_command = last_command->u_data.s_pipe.right;
		last_command->u_data.s_command.exec_data.out_type = PIPE_T;
		last_command->u_data.s_command.exec_data
			.pipe_index_out = shell->pipe_index;
	}
}

void	close_used_fds(t_shell *shell, t_ast_node *node)
{
	t_exec_data	*data;

	(void)shell;
	data = &node->u_data.s_command.exec_data;
	if (data->in_type == FILE_T)
		close(data->in_file);
	if (data->out_type == FILE_T)
		close(data->out_file);
}

void	close_unused_pipes(t_ast_node *node, t_shell *shell)
{
	int			i;
	t_exec_data	*data;

	i = 0;
	data = &node->u_data.s_command.exec_data;
	while (i < shell->pipe_count)
	{
		if (i != data->pipe_index_in && i != data->pipe_index_out)
		{
			close(shell->pipes[i][0]);
			close(shell->pipes[i][1]);
		}
		else
		{
			if (data->pipe_index_in == i)
				close(shell->pipes[i][1]);
			if (data->pipe_index_out == i)
				close(shell->pipes[i][0]);
		}
		i++;
	}
}


t_shell	init_subshell(t_shell	*shell, t_ast_node *node)
{
	t_shell	sub_shell;

	sub_shell.envp = shell->envp;
	sub_shell.exit_status = 0;
	sub_shell.line = NULL;
	sub_shell.dir = shell->dir;
	sub_shell.pipe_count = count_pipes(node);
	sub_shell.process_count = count_pipes(node) + 1;
	if (sub_shell.pipe_count == 0)
		sub_shell.process_count = 0;
	sub_shell.pipeline = false;
	sub_shell.pipe_index = 0;
	sub_shell.process_index = 0;
	sub_shell.root_node = node;
	sub_shell.subshell = NULL;
	sub_shell.parent_shell = shell;
	shell->subshell = &sub_shell;
	return (sub_shell);
}
