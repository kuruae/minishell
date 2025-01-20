/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:29:09 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/20 02:19:57 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

t_exec_error	recur_exec(t_shell *shell, t_ast_node *node);



//this function is ment to transform the status of the child in to 
//a according exex_error status
t_exec_error	return_status(int	status)
{
    int exit_status;

    if (WIFEXITED(status))
    {
        exit_status = WEXITSTATUS(status);
        if (exit_status == 0)
            return (EXEC_SUCCESS);
        else if (exit_status == 127) // 127 is the exit status for command not found
            return (EXEC_NOT_FOUND);
        else
            return (EXEC_ERR_NON_FATAL);
    }
    return (EXEC_ERR_FATAL);
}

t_exec_error	start_command(t_shell *shell, t_ast_node *node)
{
	t_exec_error	status;
	pid_t			child_pid;
	int				child_status;

	status = builtin_parent(node, shell);
	if (status != EXEC_NOT_FOUND)
		return (status);
	child_pid = fork();
	if (child_pid == -1)
		return (perror("total error: fork:"), EXEC_ERR_FATAL);
	//child process:
	if (child_pid == 0)
	{
		exec_command(shell, node);
		//normally it should not reach here
		ft_printf("Child process did not exit properly\n");
		exit(1);
	}
	waitpid(child_pid, &child_status, 0);
	return (return_status(child_status));
}

t_exec_error start_subshell(t_shell *shell, t_ast_node *node)
{	
	int		status;
	int		i;
	t_shell	subshell;
	pid_t	pid;

	i = 0;
	subshell = init_subshell(shell, node);
	if (subshell.pipe_count == 0)
	{
		pid = fork();
		if (pid == -1)
			return (EXEC_ERR_FATAL);
		if (pid == 0)
			exit(recur_exec(&subshell, node));
		waitpid(pid, &status, 0);
		return (return_status(status));
	}
	recur_exec(&subshell, node);
	while (i < subshell.process_count)
	{
		waitpid(subshell.pid[i], &status, 0);
		i++;
	}
	return (return_status(status));
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
		status = recur_exec(shell, node->data.logical_op.left);
		if (status == EXEC_ERR_FATAL)
			return (status);
		return (recur_exec(shell, node->data.logical_op.right));
	}
	else if (node->type == NODE_OR)
	{
		status = recur_exec(shell, node->data.logical_op.left);
		if (status == EXEC_SUCCESS)
			return (status);
		return (recur_exec(shell, node->data.logical_op.right));
	}
	else if (node->type == NODE_SUBSHELL)
		return (start_subshell(shell, node->data.subshell.command));
	else
		ft_printf("this version only suports pipes and commands\n");
	return (EXIT_SUCCESS);
}

t_exec_error	start_exec(t_shell *shell, t_ast_node *node)
{
	t_exec_error	status;
	int i;

	shell->process_count = count_pipes(node) + 1;
	shell->pipe_count = count_pipes(node);
	shell->pipe_index = 0;
	shell->process_index = 0;
	shell->root_node = node;
	status = recur_exec(shell, node);
	i = 0;
	while (i < shell->process_count)
	{
		waitpid(shell->pid[i], NULL, 0);
		i++;
	}
	return (status);
}
