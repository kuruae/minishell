/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@42.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:29:09 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/22 16:06:36 by jbaumfal         ###   ########.fr       */
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

/*
	This function is called when a part of the command line is in brackets
		-> this part of the command line will be executed in a subshell
			-> we will give it a new sell structure
			-> then we will start the execution of this subshell as it is a command line by itself
	

	EXAMPLE: export VAR=11 && (env | grep VAR)
			-> the part in brackets will be executed in a subshell as if there was the command line env | grep VAR
			after the subshell launched all its execution it waits fr all the child processes to finish
			- once terminated it will return the exit status of the last child process to the parent shell

*/

void			free_subshell(t_shell *shell)
{
	free(shell);
}


t_exec_error	start_subshell(t_shell *shell, t_ast_node *node)
{	
	int				child_status;
	t_shell			*subshell;
	t_exec_error	status;
	int				i;

	subshell = init_subshell(shell, node);
	status = recur_exec(subshell, node);
	if (status == EXEC_ERR_FATAL)
		return (free(subshell), status);
	i = 0;
	while (i < subshell->process_count)
	{
		waitpid(subshell->pid[i], &child_status, 0);
		analize_child_status(child_status);
		i++;
	}
	free(subshell);
	return (return_exit_status(g_sig_offset));
}


/*
	In this function we travel throught the whole AST tree (recursively) nde by node to then start the execution of them
	-> for every node the function will be recalled
		-> we check the type of the node and call the corresponding function
		
		- NODE_COMMAND: we start the command execution (start_command)
		- PIPE(|): we start the pipeline execution (start_pipeline)
			-> this will execute the whole pipeline, therefore the recursive eecution can stop here
		- AND(&&): we first start the execution of the left node and then the right node
			-> if the left nod has a fatal error we stop the execution directly
		- OR(||): we first start the execution of the left node and in case of an non fatal error the right node afterwards
			-> if the left node was successful or there was a fatal error we stop the execution directly
		- SUBSHELL((brackets)): the execution of a subshell is handled in a seperate function (start_subshell)

		IMPORTANT: unlik the real bash , with this structure we cant handle commands that connect subshells with pipes
			- in our subject subshells (brackets) were only demanded for logical operators
*/

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
		if (status == EXEC_SUCCESS || status == EXEC_ERR_FATAL)
			return (status);
		return (recur_exec(shell, node->u_data.s_logical_op.right));
	}
	else if (node->type == NODE_SUBSHELL)
		return (start_subshell(shell, node->u_data.s_subshell.command));
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
