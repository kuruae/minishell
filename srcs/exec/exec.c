/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:29:09 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/10 16:11:47 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exec_error	recur_exec(t_shell *shell, t_ast_node *node);

/*
	First we have to handle builtins 
	beacause they are not executed in a child process
	
	If the command was a builtin I set the process count to 0
	(as there is no child process needed)

	If the builtin function returns EXEC_NOT_FOUND we keep going
	-> Now we can create a fork as we will exec 
	the system commands in a child process

	The exec command functions handles the execution 
	and when successful it will make the child process exit
*/

t_exec_error	start_command(t_shell *shell, t_ast_node *node)
{
	t_exec_error	status;
	pid_t			child_pid;
	int				wait_status;

	status = prepare_command(shell, node);
	if (status != EXEC_SUCCESS)
		return (status);
	status = builtin(node, shell, node->u_data.s_command.exec_data.out_file);
	if (status != EXEC_NOT_FOUND)
		return (status);
	get_signal_exec();
	child_pid = fork();
	if (child_pid == -1)
		return (perror("total error: fork:"), EXEC_ERR_FATAL);
	shell->pid[shell->process_index++] = child_pid;
	if (child_pid == 0)
		exec_command(shell, node);
	else
	{
		close_redirections(node);
		waitpid(child_pid, &wait_status, 0);
		analize_child_status(wait_status);
	}
	return (return_exit_status(g_sig_offset));
}

t_exec_error	start_subshell(t_shell *shell, t_ast_node *node)
{	
	int				child_status;
	t_shell			subshell;
	t_exec_error	status;
	int				i;

	subshell = init_subshell(shell, node);
	status = recur_exec(&subshell, node);
	if (status == EXEC_ERR_FATAL)
		return (status);
	i = 0;
	while (i < subshell.process_count)
	{
		waitpid(subshell.pid[i], &child_status, 0);
		analize_child_status(child_status);
		i++;
	}
	return (return_exit_status(g_sig_offset));
}

t_exec_error	recur_exec(t_shell *shell, t_ast_node *node)
{
	t_exec_error	status;

	if (node->type == NODE_COMMAND)
		return (start_command(shell, node));
	else if (node->type == NODE_PIPE)
		return (start_pipeline(shell, node));
	else if (node->type == NODE_AND)
	{
		status = recur_exec(shell, node->u_data.s_logical_op.left);
		if (status == EXEC_ERR_FATAL)
			return (status);
		return (recur_exec(shell, node->u_data.s_logical_op.right));
	}
	else if (node->type == NODE_OR)
	{
		status = recur_exec(shell, node->u_data.s_logical_op.left);
		if (status == EXEC_SUCCESS)
			return (status);
		return (recur_exec(shell, node->u_data.s_logical_op.right));
	}
	else if (node->type == NODE_SUBSHELL)
		return (start_subshell(shell, node->u_data.s_subshell.command));
	else
		ft_printf("this version only suports pipes and commands\n");
	return (EXIT_SUCCESS);
}

/*
	This function cn be seen as he mainfunction of the execution.
	
	- We start by initializing the shell structure with the standard values
		- most of these values will be modified later but this way we avoid errors
	- Then i update the shell level
*/
t_exec_error	start_exec(t_shell *shell, t_ast_node *node)
{
	t_exec_error	status;
	int				i;
	int				child_status;

	init_shell(shell, node);
	status = update_shell_level(shell);
	if (status != EXEC_SUCCESS)
		return (status);
	status = recur_exec(shell, node);
	if (status == EXEC_ERR_FATAL)
		return (status);
	i = 0;
	get_signal_exec();
	while (i < shell->process_count)
	{
		waitpid(shell->pid[i], &child_status, 0);
		analize_child_status(child_status);
		i++;
	}
	return (status);
}
