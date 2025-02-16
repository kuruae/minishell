/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@42.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:00:26 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/16 04:51:10 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*
	This function is used to imitate the real env call in bash.

	- First we check the number of arguments given (only 0 arguments is allowed).
	- If no argument is given we print the whole environment.
*/

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
