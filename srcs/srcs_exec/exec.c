/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:29:09 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/18 02:57:27 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"



t_exec_error	start_exec(t_shell *shell, t_ast_node *node)
{
	pid_t			child_pid;
	int				status;

	if (node->type == NODE_COMMAND)
	{
		child_pid = fork();
		if (child_pid == -1)
			return (perror("total error: fork:"), EXEC_ERR_FATAL);
		if (child_pid == 0)
			exec_command(shell, node);
		waitpid(child_pid, &status, 0);
		if (WIFEXITED(status) == true)
			return (EXEC_SUCCESS);
	}
	else
		ft_printf("this version only suports single commands\n");
	return (EXEC_SUCCESS);
}

