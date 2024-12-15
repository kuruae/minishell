/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:00:26 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/15 17:47:43 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exec_error	ft_env(char **envp, int argc, int fd_out) // im not sure if envc should work when given with arguments
{									//or if it should return an error
	int	i;

	i = 0;
	if (argc != 0)
	{
		ft_putstr_fd("total error: env: don't take arguments\n", 2);
		return (EXEC_ERR_NON_FATAL);
	}
	// i dont add an error case for when the envp variable is missing because
	// as far as im concerned the real env bash command doesnt return an error
	if (!envp || !(envp[i]))
		return (ft_putchar_fd('\n', fd_out), EXEC_SUCCESS);
	while (envp[i])
	{
		ft_putstr_fd(envp[i], fd_out);
		ft_putchar_fd('\n', fd_out);
		i++;
	}
	return (EXEC_SUCCESS);
}
