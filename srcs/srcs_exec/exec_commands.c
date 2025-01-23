/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:28:49 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/16 16:00:52 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	When someone manually removes the PATH variable from the environment
	This function would return NULL
	However this would it make it impossible for me to know if a malloc failed or the PATH is not there
	Thats why I return the string "null" in case of a missing PATH variable 
*/
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
		return (ft_split(ft_strdup("null"), ':'));
	paths = ft_split(path_value, ':');
	return (free(path_value), paths);
}

void	setting_std_in_out(t_ast_node *node)
{
	int	in_file;
	int	out_file;

	if (node->data.command.exec_data.in_type == FILE_T)
	{
		in_file = node->data.command.exec_data.in_file;
		dup2(in_file, STDIN_FILENO);
		close(in_file);
	}
	if (node->data.command.exec_data.out_type == FILE_T)
	{
		out_file = node->data.command.exec_data.out_file;
		dup2(out_file, STDOUT_FILENO);
		close(out_file);
	}
}


t_exec_error		try_absolute_path(char **args, char **env, t_ast_node *node)
{
	if (access(args[0], F_OK) == 0) // checking if file exist and exec permission
	{
		 if (access(args[0], X_OK) == 0) // checking if file has exec permission
        {
            setting_std_in_out(node);
            execve(args[0], args, env);
        }
        else
            return (perror("total error"), EXEC_ERR_ACCESS);
	}
	return (EXEC_NOT_FOUND);
}

t_exec_error	try_command(char **paths, char **args, char **env, t_ast_node *node)
{
	char	*command_path;
	int		i;
	char	*path;

	i = 0;
	if (try_absolute_path(args, env, node) == EXEC_ERR_ACCESS)
		return (EXEC_ERR_ACCESS);
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		command_path = ft_strjoin(path, args[0]);
		free(path);
		if (!command_path)
			return (free(command_path), EXEC_ERR_FATAL);
		if (access(command_path, F_OK) == 0) // checking if file exist and exec permission
		{
			if (access(command_path, X_OK) == 0)
			{
				setting_std_in_out(node);
				execve(command_path, args, env);
			}
			else
				return (perror("total error"), free(command_path), EXEC_ERR_ACCESS);
			return (free(command_path), EXEC_SUCCESS);
		}
		i++;
		free(command_path);
	}
	return (EXEC_NOT_FOUND);
}

/* this function we use to create a new argv table that includes the command like execve is used*/

/*
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
*/

void	exec_command(t_shell *shell, t_ast_node *node)
{
    char **argv_exec;
    char **paths;
    t_exec_error status;

	if (!node || !node->data.command.command)
	{
		ft_printf("Invalid node or command\n");
		exit(1);
    }
	argv_exec = node->data.command.argv_exec;
	if (set_input_output(shell, node) == EXEC_ERR_FILE)
		exit(1);
	close_unused_pipes(node, shell);
	if (shell->pipeline == true)
	{
		status = builtin(node, shell);
		if (status != EXEC_NOT_FOUND)
			exit_exec_status(status);
	}
	paths = get_paths(*shell->envp);
	if (!paths)
		exit(1);
	status = try_command(paths, argv_exec, *shell->envp, node);
	if (status == EXEC_ERR_FATAL)
		exit(1);
	free(paths);
	if (status == EXEC_NOT_FOUND)
        ft_putstr_fd("total error: command not found\n", 2);
	exit_exec_status(status);
}
