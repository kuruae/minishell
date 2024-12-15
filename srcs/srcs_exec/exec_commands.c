/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:28:49 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/15 18:30:01 by jbaumfal         ###   ########.fr       */
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

t_exec_error try_command(char **paths, char **args, char *command, char **env)
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
	return (EXEC_NOT_FOUND);
}

t_exec_error	exec_command(t_shell *shell, t_ast_node *node, int fd_out)
{
	char		*command;
	char		**args;
	int			arg_count;
	char		**paths;
	t_exec_error	status;


	command = node->data.command.command;
	args = node->data.command.args;
	arg_count = node->data.command.arg_count;
	status = builtin(command, args, arg_count, fd_out, shell->envp);
	if (status != EXEC_NOT_FOUND) // only continues when the command wasnt found in the builtins
		return (status);
	paths = get_paths(*shell->envp);
	if (!paths)
		return (EXEC_ERR_FATAL);
	status = try_command(paths, args, command, *shell->envp);
	return (status);
}
