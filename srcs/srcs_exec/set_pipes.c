/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_pipes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:16:23 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/08 16:38:06 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_pipes(t_ast_node *node, t_shell *shell)
{
	t_exec_data	*data;

	data = &node->u_data.s_command.exec_data;
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
