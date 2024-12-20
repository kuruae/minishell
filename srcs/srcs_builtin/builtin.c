/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:24:56 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/20 02:20:25 by jbaumfal         ###   ########.fr       */
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


//this function is for the builtin commands that have to be executed in the parrent
t_exec_error	builtin_parent(t_ast_node *node, t_shell *shell)
{
	t_exec_error	status;
	char			*command;
	char			**args;
	int				argc;
	char			***envp;

	command = node->data.command.command;
	args = node->data.command.args;
	argc = node->data.command.arg_count;
	envp = shell->envp;
	if (!command) 
	{
		ft_printf("Command or args is NULL\n");
		return EXEC_ERR_FATAL;
	}
	if (ft_strcmp(command, "cd") == 0)
		status = ft_cd(args, argc, &shell->dir, *envp);
	else if (ft_strcmp(command, "export") == 0)
		status = ft_export(args, argc, envp);
	else if (ft_strcmp(command, "unset") == 0)
		status = ft_unset(args, argc, *envp);
	else
		status = EXEC_NOT_FOUND;
	return (status);
}

t_exec_error	builtin(t_ast_node *node, t_shell *shell)
{
	t_exec_error	status;
	char			*command;
	char			**args;
	int				argc;
	char			***envp;

	command = node->data.command.command;
	args = node->data.command.args;
	argc = node->data.command.arg_count;
	envp = shell->envp;
	if (!command) 
	{
		ft_printf("Command or args is NULL\n");
		return EXEC_ERR_FATAL;
	}
	(void)argc;
	if (ft_strcmp(command, "echo") == 0)
		status = ft_echo(args, argc);
	else if (ft_strcmp(command, "pwd") == 0)
		status = ft_pwd(&shell->dir);
	else if (ft_strcmp(command, "env") == 0)
		status = ft_env(*envp, argc);
	// else if (ft_strcmp(command, "exit") == 0)
	// 	status = ft_exit(args[1]);
	else
		status = EXEC_NOT_FOUND;
	//ft_printf("end of builtin function\n");
	return (status);
}

