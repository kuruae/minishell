/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:24:56 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/15 01:44:02 by jbaumfal         ###   ########.fr       */
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

t_bi_error	builtin(char *command, char **args, int argc, int fd_out, char ***envp)
{
	t_directory	dir;
	t_bi_error	status;

	(void)argc;
	if (ft_strcmp(command, "echo") == 0)
		status = ft_echo(args[1], fd_out);
	else if (ft_strcmp(command, "cd") == 0)
		status = ft_cd(args[1], &dir, *envp);
	else if (ft_strcmp(command, "pwd") == 0)
		status = ft_pwd(&dir, fd_out);
	else if (ft_strcmp(command, "export") == 0)  // for this we need to know how we handle our child processes
		status = ft_export(args[1], envp);
	else if (ft_strcmp(command, "unset") == 0)
		status = ft_unset(args[1], *envp);
	else if (ft_strcmp(command, "env") == 0)
		status = ft_env(*envp, fd_out);
	else if (ft_strcmp(command, "exit") == 0)
		status = ft_exit(args[1]);
	else
		status = BI_ERR_NO_BUILT_IN;
	return (status);
}

