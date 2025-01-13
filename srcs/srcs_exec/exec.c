/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:29:09 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/13 16:17:55 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


//this function is ment to transform the status of the child in to 
//a according exex_error status
t_exec_error	return_status(int	status)
{
	int exit_status;

	if (WIFEXITED(status) == true)
			return (EXEC_SUCCESS);
	else 
		exit_status = WEXITSTATUS(status);
	g_sig_offset = exit_status;
	if (exit_status == 128)
		return (EXEC_ERR_FATAL);
	else
		return (EXEC_ERR_NON_FATAL);
}

t_exec_error	start_exec(t_shell *shell, t_ast_node *node)
{
	pid_t			child_pid;
	int				child_status;
	t_exec_error	status;

	if (node->type == NODE_COMMAND)
	{
		status = builtin_parent(node, shell);
		if (status != EXEC_NOT_FOUND)
			return (status);
		child_pid = fork();
		if (child_pid == -1)
			return (perror("total error: fork:"), EXEC_ERR_FATAL);
		//child process:
		if (child_pid == 0)
		{
//			ft_printf("Child process started\n");
			exec_command(shell, node);
			//normally it should not reach here
			ft_printf("Child process did not exit properly\n");
			exit(1);
		}
		waitpid(child_pid, &child_status, 0);
		return (return_status(child_status));
	}
	else
		ft_printf("this version only suports single commands\n");
	return (EXEC_SUCCESS);
}