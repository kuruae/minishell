/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:28:49 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/17 12:28:10 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_paths(char **env)
{
	int		i;
	char	*path_value;
	char	**paths;

	i = 0;
	path_value = NULL;
	if (!env || !*env)
		return (NULL);
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path_value = ft_strdup(env[i] + 5);
			if (!path_value)
				return (NULL);
			break ;
		}
		i++;
	}
	if (!path_value)
		return (NULL);
	paths = ft_split(path_value, ':');
	return (free(path_value), paths);
}

t_exec_error	try_command(char **paths, char **args, char *command, char **env)
{
	char	*command_path;
	int		i;
	char	*path;

	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		command_path = ft_strjoin(path, command);
		free(path);
		if (!command_path)
			return (free(command_path), EXEC_ERR_FATAL);
		if (access(command_path, F_OK | X_OK) == 0) // checking if file exist and exec permission
		{
			execve(command_path, args, env);
			return (free(command_path), EXEC_SUCCESS);
		}
		i++;
		free(command_path);
	}
	ft_putstr_fd("total error: command not found\n", 2);
	return (EXEC_NOT_FOUND);
}

/* this function we use to create a new argv table that includes the command like execve is used*/

char	**transform_args(char **args, char	*command, int argc)
{
	char	**argv;
	int		i;

	argv = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!argv)
		return (NULL);
	argv[0] = ft_strdup(command);
	if (!argv[0])
		return (NULL);	
	i = 1;
	while (i <= argc)
	{
		argv[i] = ft_strdup(args[i - 1]);
		if (!argv[i])
			return (NULL);
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

t_exec_error	exec_command(t_shell *shell, t_ast_node *node, int fd_out)
{
	char			*command;
	char			**args;
	int				argc;
	char			**paths;
	t_exec_error	status;


	command = node->data.command.command;
	argc = node->data.command.arg_count;
	//first check if command is builtin
	status = builtin(command, node->data.command.args, argc, fd_out, shell->envp);
	if (status != EXEC_NOT_FOUND)
		return (status);
	// only continues when the command wasnt found in the builtins
	// First we have to adjust the argv so that it includes the comment
	args = transform_args(node->data.command.args, command, argc);
	if (!args)
		return (free_all(args), EXEC_ERR_FATAL);
	argc += 1; //adjusting argc
	paths = get_paths(*shell->envp);
	if (!paths)
		return (EXEC_ERR_FATAL);
	shell->pid[0] = fork();
	if (shell->pid[0] == -1)
		return (perror("total error: fork:"), EXEC_ERR_FATAL);
	if (shell->pid[0] == 0)
	{
		if (node->redirections)
		{
			dup2(node->redirections->fd_redir, STDOUT_FILENO);
			close(node->redirections->fd_redir);
		}
		status = try_command(paths, args, command, *shell->envp);
	}
	waitpid(shell->pid[0], NULL, 0);
	free(paths);
	return (status);
}
