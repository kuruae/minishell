/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 23:45:35 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/05 15:45:36 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_exec_status(t_exec_error	status)
{
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
	ft_printf("missing status in exit_exec_status function\n");
	exit(1);
}

void	link_pipe(t_ast_node *node, t_shell *shell)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_ast_node	*last_command;

	left = node->data.pipe.left;
	right = node->data.pipe.right;
	if (left->type == NODE_COMMAND)
	{
		left->data.command.exec_data.out_type = PIPE_T;
		left->data.command.exec_data.pipe_index_out = shell->pipe_index;
	}
	if (right->type == NODE_COMMAND)
	{
		right->data.command.exec_data.in_type = PIPE_T;
		right->data.command.exec_data.pipe_index_in = shell->pipe_index;
	}
	if (left->type == NODE_PIPE)
	{
		last_command = left;
		while (last_command->type == NODE_PIPE)
			last_command = last_command->data.pipe.right;
		last_command->data.command.exec_data.out_type = PIPE_T;
		last_command->data.command.exec_data.pipe_index_out = shell->pipe_index;
	}
}

void	close_used_fds(t_shell *shell, t_ast_node *node)
{
	t_exec_data	*data;

	(void)shell;
	data = &node->data.command.exec_data;
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
	data = &node->data.command.exec_data;
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
	sub_shell.process_count = count_pipes(node) + 1;
	sub_shell.pipe_count = count_pipes(node);
	if (shell->pipe_count == 1)
		shell->process_count = 0;
	sub_shell.pipeline = false;
	sub_shell.pipe_index = 0;
	sub_shell.process_index = 0;
	sub_shell.root_node = node;
	return (sub_shell);
}
