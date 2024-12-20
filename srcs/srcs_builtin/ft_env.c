/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:00:26 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/18 02:49:17 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exec_error	ft_env(char **envp, int argc) // im not sure if envc should work when given with arguments
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
		return (ft_putchar_fd('\n', STDOUT_FILENO), EXEC_SUCCESS);
	while (envp[i])
	{
		ft_putstr_fd(envp[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
	return (EXEC_SUCCESS);
}
