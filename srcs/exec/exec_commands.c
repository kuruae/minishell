/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@42.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:28:49 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/16 05:20:26 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	This function is used to set the in and outfile as the STDIN and STDOUT
		- for this we use the dup2 function
		- after duplicating them we can close the original file directors
*/

void	dup2_infile_outfile(t_ast_node *node, t_shell *shell)
{
	int	in_file;
	int	out_file;

	(void)shell;
	if (node->u_data.s_command.exec_data.in_type == FILE_T)
	{
		in_file = node->u_data.s_command.exec_data.in_file;
		dup2(in_file, STDIN_FILENO);
		close(in_file);
	}
	if (node->u_data.s_command.exec_data.out_type == FILE_T)
	{
		out_file = node->u_data.s_command.exec_data.out_file;
		dup2(out_file, STDOUT_FILENO);
		close(out_file);
	}
}

t_exec_error	try_absolute_path(char **args, t_shell *shell, t_ast_node *node)
{
	if (access(args[0], F_OK) == 0)
	{
		if (access(args[0], X_OK) == 0)
		{
			dup2_infile_outfile(node, shell);
			close_used_fds(shell, node);
			execve(args[0], args, *shell->envp);
		}
		else
			return (perror("total error"), EXEC_ERR_ACCESS);
	}
	return (EXEC_NOT_FOUND);
}


/*
	this function cheks if the command that we have and that we found in the path, is executable
	- if it is we execute the command
		- if this is the case we launch the function that sets the in and outfile as the STDIN and STDOUT
		- we then close the used file descriptors and execute the command
	- if the command is not executable we return an error
	
*/

t_exec_error	launch(char *pth, char **args, t_shell *shell, t_ast_node *node)
{
	if (access(pth, X_OK) == 0)
	{
		dup2_infile_outfile(node, shell);
		close_used_fds(shell, node);
		execve(pth, args, *shell->envp);
	}
	else
		return (perror("total error"), free(pth), EXEC_ERR_ACCESS);
	return (free(pth), EXEC_SUCCESS);
}
/*
	This function is used to try to find the command in the PATH environment variable
	- First we check if the command is an absolute path 
		- ( in that case it will be executed or in case of an ACESS ERROR we return the error)
	- Then we get the paths from the PATH environment variable
		-> those are the directories where the shell should look for the command
	- We then loop through the paths and try to find the command
		- If the command is found we start the launch function that tries executes the command
*/
t_exec_error	try_com(char **args, t_shell *shell, t_ast_node *node)
{
	char	*command_path;
	int		i;
	char	**paths;
	char	*path;

	i = 0;
	if (try_absolute_path(args, shell, node) == EXEC_ERR_ACCESS)
		return (EXEC_ERR_ACCESS);
	paths = get_paths(*shell->envp);
	if (!paths)
		exit(1);
	while (paths[i])
	{
		path = ft_strjoin(paths[i++], "/");
		if (!path)
			return (free_all(paths), EXEC_ERR_FATAL);
		command_path = ft_strjoin(path, args[0]);
		free(path);
		if (!command_path)
			return (free_all(paths), free(command_path), EXEC_ERR_FATAL);
		if (access(command_path, F_OK) == 0)
			return (free_all(paths), launch(command_path, args, shell, node));
		free(command_path);
	}
	return (free_all(paths), EXEC_NOT_FOUND);
}


/*
	This is the function that is called in the child process to launch the command execution
	
	- as the child shouldnt react to the signals like the parrent process we reset the signals

	- we then close unused pipes. As they exist in the child process but are not needed

	- if this command is part of a pipeline we have to check if it is a builtin 
		- normaly we execute builtins in the parrent process but 
			in a pipeline this is problematic as we need to manipulate the STDIN and STDOUT
	- if the command is not a builtin we start the try_com function that tries to find and execute the command
		- in case they are executed the process is killed and the function therefore stops here
	- if the command is not found we print an error message
*/

void	exec_command(t_shell *shell, t_ast_node *node)
{
	char			**argv_exec;
	t_exec_error	status;
	int				outfile;

	outfile = node->u_data.s_command.exec_data.out_file;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!node || !node->u_data.s_command.command)
	{
		ft_printf("Invalid node or command\n");
		exit_exec_status(EXEC_ERR_NON_FATAL, shell);
	}
	argv_exec = node->u_data.s_command.argv_exec;
	close_unused_pipes(node, shell);
	if (shell->pipeline == true)
	{
		status = builtin(node, shell, outfile);
		if (status != EXEC_NOT_FOUND)
			exit_exec_status(status, shell);
	}
	status = try_com(argv_exec, shell, node);
	if (status == EXEC_NOT_FOUND)
		print_error(NULL, argv_exec[0], "command not found");
	close_redirections(node);
	exit_exec_status(status, shell);
}
