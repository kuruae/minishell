/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:25:20 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/15 17:47:43 by jbaumfal         ###   ########.fr       */
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

t_exec_error	ft_exit(char *arg)
{
	int	arg_n;

	if (!arg)
		exit(g_sig_offset);
	if (!ft_is_num(arg))
	{
		ft_putstr_fd("exit: numeric argument required\n", STDERR_FILENO);
		g_sig_offset = 103;
		exit(103);
	}
	arg_n = ft_atoi(arg);
	if (arg_n < 0 || arg_n > 255) // check if the given number is a valid exit status
	{
		ft_putstr_fd("exit: invalid exit status\n", STDERR_FILENO); // not sure what to do in this case
		g_sig_offset = 184;
	}
	g_sig_offset = arg_n; // setting global variable
	exit(arg_n);
	return (EXEC_SUCCESS);
}
