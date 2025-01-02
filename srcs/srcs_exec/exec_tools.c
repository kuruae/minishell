/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 23:45:35 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/02 20:40:57 by jbaumfal         ###   ########.fr       */
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
	if (status == EXEC_ERR_ACESS)
		exit(1);
	if (status == EXEC_ERR_FILE)
		exit(1);
	ft_printf("missing status in exit_exec_status function\n");
}

void	link_pipe(t_ast_node	*node)
{
	t_ast_node	*left;
	t_ast_node	*right;

	left = node->data.pipe.left;
	right = node->data.pipe.left;
	if (left->type == NODE_COMMAND)
	{
		left->data.command.exec_data.out_type = PIPE_T;
		left->data.command.exec_data.out_file = node->data.pipe.pipe[1];
	}
	if (right->type == NODE_COMMAND)
	{
		left->data.command.exec_data.in_type = PIPE_T;
		left->data.command.exec_data.in_file = node->data.pipe.pipe[0];
	}
	if (left->type == NODE_PIPE)
	{
		left = &*left->data.pipe.right;
		left->data.command.exec_data.out_type = PIPE_T;
		left->data.command.exec_data.out_file = node->data.pipe.pipe[1];
	}
}