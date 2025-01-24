/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 18:39:42 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/24 19:41:57 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void close_used_pipes(t_shell *shell, t_ast
	if (data->in_type == PIPE_T)
		close(shell->pipes[data->pipe_index_in][1]);
	if (data->out_type == PIPE_T)
		close(shell->pipes[data->pipe_index_out][0]);