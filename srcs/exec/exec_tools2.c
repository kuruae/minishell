/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 18:39:42 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/21 20:51:20 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_used_pipes(t_shell *shell, t_ast_node *node)
{
	t_exec_data	*data;

	data = &node->u_data.s_command.exec_data;
	if (data->in_type == PIPE_T)
		close(shell->pipes[data->pipe_index_in][0]);
	if (data->out_type == PIPE_T)
		close(shell->pipes[data->pipe_index_out][1]);
}

/*
	With this function we can determine the status of the child process (as prior to this the g_sig_ofset is set to the status of the child process)
	- We than can transform the exit status 
*/

t_exec_error	return_exit_status(int g_sig_offset)
{
	if (g_sig_offset == 0)
		return (EXEC_SUCCESS);
	if (g_sig_offset == 1)
		return (EXEC_ERR_NON_FATAL);
	if (g_sig_offset == 2)
		return (EXEC_ERR_NON_FATAL);
	if (g_sig_offset == 126)
		return (EXEC_ERR_ACCESS);
	if (g_sig_offset == 127)
		return (EXEC_NOT_FOUND);
	if (g_sig_offset == 128)
		return (EXEC_ERR_FATAL);
	return (EXEC_SUCCESS);
}


/*
	This function is launched after the waitpid function was executed which stored the status of the child in the child_status variable
	- First we check if the child was terminated
		- if so we check if the signal was SIGQUIT and set the g_sig_offset to 131
	- In all other casees we set the g_sig_offset to the exit status of the child
*/

void	analize_child_status(int child_status)
{
	if (WIFSIGNALED(child_status))
	{
		if (WTERMSIG(child_status) == SIGQUIT)
				g_sig_offset = 131;
	}
	else if (WIFEXITED(child_status))
		g_sig_offset = WEXITSTATUS(child_status);
}

/*
	This is a flexible function to print errors in the same way like the real bash

	Fomat: total error: 'situation': 'suspect': 'error'
		- situation: the situation in which the error occured
		- suspect: the command / string that caused the error
		- error: the error message
*/

void	print_error(char *situation, char *suspect, char *error)
{
	ft_putstr_fd("total error: ", STDERR_FILENO);
	if (situation)
	{
		ft_putstr_fd(situation, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_printf("'");
	ft_putstr_fd(suspect, STDERR_FILENO);
	ft_putstr_fd("': ", STDERR_FILENO);
	ft_putendl_fd(error, STDERR_FILENO);
}
