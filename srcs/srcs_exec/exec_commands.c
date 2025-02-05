/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:28:49 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/05 17:55:29 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup2_infile_outfile(t_ast_node *node, t_shell *shell)
{
	int	in_file;
	int	out_file;

	(void)shell;
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

t_exec_error	try_absolute_path(char **args, t_shell *shell, t_ast_node *node)
{
	if (access(args[0], F_OK) == 0)
	{
		if (access(args[0], X_OK) == 0)
		{
			dup2_infile_outfile(node, shell);
			execve(args[0], args, *shell->envp);
		}
		else
			return (perror("total error"), EXEC_ERR_ACCESS);
	}
	return (EXEC_NOT_FOUND);
}

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

void	exec_command(t_shell *shell, t_ast_node *node)
{
	char			**argv_exec;
	t_exec_error	status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!node || !node->data.command.command)
	{
		ft_printf("Invalid node or command\n");
		exit(1);
	}
	argv_exec = node->data.command.argv_exec;
	close_unused_pipes(node, shell);
	if (shell->pipeline == true)
	{
		status = builtin(node, shell, node->data.command.exec_data.out_file);
		if (status != EXEC_NOT_FOUND)
			exit_exec_status(status, shell);
	}
	status = try_com(argv_exec, shell, node);
	if (status == EXEC_ERR_FATAL)
		exit(1);
	if (status == EXEC_NOT_FOUND)
		ft_putstr_fd("total error: command not found\n", 2);
	exit_exec_status(status, shell);
}
