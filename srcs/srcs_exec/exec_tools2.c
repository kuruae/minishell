/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 18:39:42 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/30 00:58:23 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void close_used_pipes(t_shell *shell, t_ast_node *node)
{
	t_exec_data	*data;

	data = &node->data.command.exec_data;
	if (data->in_type == PIPE_T)
		close(shell->pipes[data->pipe_index_in][1]);
	if (data->out_type == PIPE_T)
		close(shell->pipes[data->pipe_index_out][0]);
}

bool	is_directory(char *path)
{
	int i;

	if (!path)
		return (false);
	i = 0;
	while (path[i + 1])
		i++;
	if (path[i] == '/')
	{
		g_sig_offset = 126;
		return (true);
	}
	else
		return (false);
}