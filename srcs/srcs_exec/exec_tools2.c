/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 18:39:42 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/02 17:14:10 by jbaumfal         ###   ########.fr       */
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

t_exec_error	return_exit_status(int g_sig_offset)
{
	if (g_sig_offset == 0)
		return (EXEC_SUCCESS);
	if (g_sig_offset == 1)
		return (EXEC_ERR_NON_FATAL);
	if (g_sig_offset == 126)
		return (EXEC_ERR_ACCESS);
	if (g_sig_offset == 127)
		return (EXEC_NOT_FOUND);
	if (g_sig_offset == 128)
		return (EXEC_ERR_FATAL);
	return (EXEC_SUCCESS);
}

void analize_child_status(int child_status)
{
	if (WIFSIGNALED(child_status))
	{
    	if (WTERMSIG(child_status) == SIGQUIT)
				g_sig_offset = 131;
	}
	else if (WIFEXITED(child_status))
        g_sig_offset = WEXITSTATUS(child_status);
}