/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:24:56 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/05 17:10:59 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(char **arg)
{
	int	i;

	i = 0;
	if (!arg)
		return ;
	while (arg[i])
		free(arg[i++]);
	free (arg);
}

/*
This function is for the builtin commands.
I compare each builtin with the command given in the node.
Once I find a match I call the corresponding function.
If no match is found I return EXEC_NOT_FOUND.
*/

void	set_sig_offset(t_exec_error status)
{
	if (status == EXEC_ERR_FATAL)
		g_sig_offset = 128;
	if (status == EXEC_SUCCESS)
		g_sig_offset = 0;
	if (status == EXEC_ERR_NON_FATAL)
		g_sig_offset = 1;
	if (status == EXEC_ERR_ACCESS)
		g_sig_offset = 1;
	if (status == EXEC_ERR_FILE)
		g_sig_offset = 1;
	if (status == EXEC_NOT_FOUND)
		g_sig_offset = 127;
}

t_exec_error	builtin(t_ast_node *node, t_shell *shell, int fd_out)
{
	t_exec_error	status;
	char			*command;
	char			**args;
	int				argc;

	command = node->data.command.command;
	args = node->data.command.args;
	argc = node->data.command.arg_count;
	status = EXEC_NOT_FOUND;
	if (ft_strcmp(command, "cd") == 0)
		status = ft_cd(args, argc, &shell->dir, shell->envp);
	else if (ft_strcmp(command, "export") == 0)
		status = ft_export(args, argc, shell->envp);
	else if (ft_strcmp(command, "unset") == 0)
		status = ft_unset(args, argc, *shell->envp);
	else if (ft_strcmp(command, "echo") == 0)
		status = ft_echo(args, argc, fd_out);
	else if (ft_strcmp(command, "pwd") == 0)
		status = ft_pwd(&shell->dir, fd_out);
	else if (ft_strcmp(command, "env") == 0)
		status = ft_env(*shell->envp, argc, fd_out);
	else if (ft_strcmp(command, "exit") == 0)
		status = ft_exit(args, argc);
	set_sig_offset(status);
	return (status);
}
