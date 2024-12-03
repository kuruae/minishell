// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   run_commands.c                                     :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/10/10 17:56:25 by jbaumfal          #+#    #+#             */
// /*   Updated: 2024/12/03 15:24:10 by jbaumfal         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"

// char	**get_path_env(char **env)
// {
// 	int		i;
// 	char	*path_value;
// 	char	**paths;

// 	i = 0;
// 	path_value = NULL;
// 	if (!env || !*env)
// 		return (NULL);
// 	while (env[i] != NULL)
// 	{
// 		if (ft_strncmp(env[i], "PATH=", 5) == 0)
// 		{
// 			path_value = ft_strdup(env[i] + 5);
// 			if (!path_value)
// 				return (NULL);
// 			break ;
// 		}
// 		i++;
// 	}
// 	if (!path_value)
// 		return (NULL);
// 	paths = ft_split(path_value, ':');
// 	return (free(path_value), paths);
// }

// void	free_paths(char **argv, char **paths, char *command_path)
// {
// 	ft_free_tab(argv);
// 	ft_free_tab(paths);
// 	free(command_path);
// }

// void	find_command(char **paths, char **argv, char *exec, char **env)
// {
// 	char	*command_path;
// 	int		i;
// 	char	*path;

// 	i = 0;
// 	while (paths[i])
// 	{
// 		path = ft_strjoin(paths[i], "/");
// 		command_path = ft_strjoin(path, exec);
// 		free(path);
// 		if (!command_path)
// 		{
// 			free_paths(argv, paths, command_path);
// 			exit_error("str_join failed\n");
// 		}
// 		if (access(command_path, F_OK | X_OK) == 0)
// 		{
// 			execve(command_path, argv, env);
// 			free_paths(argv, paths, command_path);
// 			exit_error("Not Succesfull excecuting command\n");
// 		}
// 		i++;
// 		free(command_path);
// 	}
// }

// void	execute(char *command, char **env)
// {
// 	char	**paths;
// 	char	**argv;
// 	char	*exec;

// 	paths = get_path_env(env);
// 	if (!paths)
// 		exit_error("Failed getting path\n");
// 	argv = ft_split(command, ' ');
// 	if (!argv)
// 	{
// 		ft_free_tab(paths);
// 		return ;
// 	}
// 	exec = argv[0];
// 	find_command(paths, argv, exec, env);
// 	ft_free_tab(argv);
// 	ft_free_tab(paths);
// 	exit_error("Command not found\n");
// }
