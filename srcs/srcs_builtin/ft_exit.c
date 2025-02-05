/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:25:20 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/28 13:38:26 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_is_num(char *str) // function that checks if a string consist only of digits (sensitive to whitespace at the end)
{
	int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

t_exec_error	ft_exit(char **args, int argc)
{
	int	arg_n;

	if (!args)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		exit(0);
	}
	if (argc > 1)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		ft_putstr_fd("total error: exit: too many arguments\n", STDERR_FILENO);
		exit(1);
	}
	if (!ft_is_num(args[0]))
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		ft_putstr_fd("exit: numeric argument required\n", STDERR_FILENO);
		g_sig_offset = 2;
		exit(2);
	}
	arg_n = ft_atoi(args[0]);
	if (arg_n < 0 || arg_n > 255) // check if the given number is a valid exit status
		arg_n = arg_n % 265;
	g_sig_offset = arg_n; // setting global variable
	exit(arg_n);
	return (EXEC_SUCCESS);
}
