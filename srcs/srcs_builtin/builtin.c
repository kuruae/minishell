/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:24:56 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/04 15:19:43 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin(char *command, char *arg, int fd_out)
{
	t_directory dir;

	if (ft_strcmp(command, "echo") == 0)
		ft_echo(arg);
	else if (ft_strcmp(command, "cd") == 0)
		ft_cd(&dir);
	else if (ft_strcmp(command, "pwd") == 0)
		ft_pwd(&dir, fd_out);
	else if (ft_strcmp(command, "export") == 0)
		ft_export();
	else if (ft_strcmp(command, "unset") == 0)
		ft_unset();
	else if (ft_strcmp(command, "env") == 0)
		ft_env();
	else if (ft_strcmp(command, "exit") == 0)
		ft_exit();
	else
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS)
}
