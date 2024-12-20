/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:28:49 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/20 03:19:25 by jbaumfal         ###   ########.fr       */
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

void	setting_std_in_out(t_ast_node *node)
{
	int	in_file;
	int	out_file;

	if (node->data.command.exec_data.in_redir == true)
	{
		in_file = node->data.command.exec_data.in_file;
		ft_printf("fd_in: %d\n", in_file);
		dup2(in_file, STDIN_FILENO);
		close(in_file);
	}
	if (node->data.command.exec_data.out_redir == true)
	{
		out_file = node->data.command.exec_data.out_file;
		dup2(out_file, STDOUT_FILENO);
		close(out_file);
	}
}

t_exec_error	try_command(char **paths, char **args, char **env, t_ast_node *node)
{
	char	*command_path;
	int		i;
	char	*path;

	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		command_path = ft_strjoin(path, args[0]);
		free(path);
		if (!command_path)
			return (free(command_path), EXEC_ERR_FATAL);
		if (access(command_path, F_OK | X_OK) == 0) // checking if file exist and exec permission
		{
			setting_std_in_out(node);
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

void	exec_command(t_shell *shell, t_ast_node *node)
{
    //ft_printf("Entered exec_command\n");
    
    char *command;
    char **args;
    int argc;
    char **paths;
    t_exec_error status;

	if (!node || !node->data.command.command)
	{
		ft_printf("Invalid node or command\n");
		exit(1);
    }
	command = node->data.command.command;
	argc = node->data.command.arg_count;
	if (set_input_output(shell, node) == EXEC_ERR_FILE)
			exit(1);
	//first check if command is builtin
	status = builtin(node, shell);
//	ft_printf("Builtin status: %d\n", status);
	if (status != EXEC_NOT_FOUND)
		exit_exec_status(status);
	// only continues when the command wasnt found in the builtins
	// First we have to adjust the argv so that it includes the comment
	args = transform_args(node->data.command.args, command, argc);
	if (!args)
		exit(1);
	argc += 1; //adjusting argc
	paths = get_paths(*shell->envp);
	if (!paths)
		exit(1);
//	ft_printf("starting command execution\n");
	status = try_command(paths, args, *shell->envp, node);
	if (status == EXEC_ERR_FATAL)
		exit(1);
	free(paths);
	exit(82);
}
