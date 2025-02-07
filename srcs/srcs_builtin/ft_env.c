/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:00:26 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/07 20:45:51 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exec_error	ft_env(char **envp, int argc, int fd_out)
{
	int	i;

	i = 0;
	if (argc != 0)
	{
		ft_putstr_fd("total error: env: doesn't take arguments\n", 2);
		return (EXEC_ERR_NON_FATAL);
	}
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
