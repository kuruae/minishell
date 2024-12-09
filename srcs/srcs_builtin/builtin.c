/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:24:56 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/09 21:47:02 by jbaumfal         ###   ########.fr       */
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

int	builtin(char *command, char *arg, int fd_out, char ***envp) // this function is ment to receive a joined string of all the arguments after the function
{
	t_directory dir;

	if (ft_strcmp(command, "echo") == 0)
		ft_echo(arg, fd_out);
	else if (ft_strcmp(command, "cd") == 0)
		ft_cd(arg, &dir, *envp);
	else if (ft_strcmp(command, "pwd") == 0)
		ft_pwd(&dir, fd_out);
	else if (ft_strcmp(command, "export") == 0)  // for this we need to know how we handle our child processes
		ft_export(arg, envp);
	else if (ft_strcmp(command, "unset") == 0)
		ft_unset(arg, *envp);
	else if (ft_strcmp(command, "env") == 0)
		ft_env(*envp, fd_out);
	else if (ft_strcmp(command, "exit") == 0)
		ft_exit(arg);
	else
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
