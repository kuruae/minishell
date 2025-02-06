/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 15:32:08 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/06 14:35:54 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	When someone manually removes the PATH variable from the environment
	This function would return NULL
	However this would it make it impossible for me to know,
    if a malloc failed or the PATH is not there
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

t_exec_error	update_env_var(char *extension, char *value, char ***envp)
{
	char			*arg;
	char			**args;
	t_exec_error	status;

	(void)envp;
	arg = ft_strjoin(extension, value);
	if (!arg)
		return (EXEC_ERR_FATAL);
	args = malloc(sizeof(char *) * 2);
	if (!args)
		return (EXEC_ERR_FATAL);
	args[0] = arg;
	args[1] = NULL;
	status = ft_export(args, 1, envp);
	free(arg);
	free(args);
	return (status);
}

t_exec_error	update_shell_level(t_shell *shell)
{
	int				old_level;
	int				new_level;
	char			*new_level_str;
	t_exec_error	status;

	old_level = ft_atoi(getenv("SHLVL"));
	new_level = old_level + 1;
	shell->shell_level = new_level;
	new_level_str = ft_itoa(new_level);
	if (!new_level_str)
		return (EXEC_ERR_FATAL);
	status = update_env_var("SHLVL=", new_level_str, shell->envp);
	free(new_level_str);
	return (status);
}

void	init_shell(t_shell *shell, t_ast_node *node)
{
	shell->pipe_count = count_pipes(node);
	shell->process_count = count_pipes(node) + 1;
	if (shell->pipe_count == 0)
		shell->process_count = 0;
	shell->pipe_index = 0;
	shell->process_index = 0;
	shell->root_node = node;
	shell->pipeline = false;
	shell->subshell = NULL;
	shell->parent_shell = NULL;
	update_env_var("_=", "/usr/bin/env", shell->envp);
}

void	close_redirections(t_ast_node *node)
{
	if (node->data.command.exec_data.in_type == FILE_T)
		close(node->data.command.exec_data.in_file);
	if (node->data.command.exec_data.out_type == FILE_T)
		close(node->data.command.exec_data.out_file);
}
