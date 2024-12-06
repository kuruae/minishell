/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:00:26 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/06 15:32:58 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **envp, int fd_out)
{
	int	i;

	i = 0;
	// i dont add an error case for when the envp variable is missing because
	// as far as im concerned the real env bash command doesnt return an error
	if (!envp || !(envp[i]))
		return (ft_putchar_fd('\n', fd_out), EXIT_SUCCESS);
	while (envp[i])
	{
		ft_putstr_fd(envp[i], fd_out);
		ft_putchar_fd('\n', fd_out);
		i++;
	}
	return (EXIT_SUCCESS);
}
