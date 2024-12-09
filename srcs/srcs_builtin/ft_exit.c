/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:25:20 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/09 21:52:25 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_is_num(char *str) // function that checks if a string consist only of digits (sensitive to whitespace at the end)
{
	int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

int	ft_exit(char *arg)
{
	int	arg_n;

	if (!arg)
		exit(g_sig_offset);
	if (!ft_is_num(arg))
		return (ft_putstr_fd("exit: numeric argument required\n", STDERR_FILENO), EXIT_FAILURE);
	arg_n = ft_atoi(arg);
	if (arg_n < 0 || arg_n > 255) // check if the given number is a valid exit status
		return (ft_putstr_fd("exit: invalid exit status\n", STDERR_FILENO), EXIT_FAILURE); // not sure what to do in this case
	g_sig_offset = arg_n; // setting global variable
	exit(arg_n);
	return (EXIT_SUCCESS);
}
