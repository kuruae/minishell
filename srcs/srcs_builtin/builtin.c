/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:24:56 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/18 02:50:21 by jbaumfal         ###   ########.fr       */
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

t_exec_error	builtin(char *command, char **args, int argc, char ***envp)
{
	t_directory		dir;
	t_exec_error	status;

	ft_printf("Starting builtin\nCommand:%s\nArg1:%s\n", command, args[0]);
	(void)argc;
	if (ft_strcmp(command, "echo") == 0)
		status = ft_echo(args, argc);
	else if (ft_strcmp(command, "cd") == 0)
		status = ft_cd(args, argc, &dir, *envp);
	else if (ft_strcmp(command, "pwd") == 0)
		status = ft_pwd(&dir);
	else if (ft_strcmp(command, "export") == 0)
		status = ft_export(args, argc, envp);
	else if (ft_strcmp(command, "unset") == 0)
		status = ft_unset(args, argc, *envp);
	else if (ft_strcmp(command, "env") == 0)
		status = ft_env(*envp, argc);
	// else if (ft_strcmp(command, "exit") == 0)
	// 	status = ft_exit(args[1]);
	else
		status = EXEC_NOT_FOUND;
	ft_printf("end of builtin function\n");
	return (status);
}

