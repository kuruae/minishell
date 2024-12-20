/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:29:09 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/20 02:21:08 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"



t_exec_error	start_exec(t_shell *shell, t_ast_node *node)
{
	pid_t			child_pid;
	int				status;

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
		waitpid(child_pid, &status, 0);
		if (WIFEXITED(status) == true)
			return (EXEC_SUCCESS);
	}
	else
		ft_printf("this version only suports single commands\n");
	return (EXEC_SUCCESS);
}

