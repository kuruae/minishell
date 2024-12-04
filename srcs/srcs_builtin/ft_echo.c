/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:07:48 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/04 15:15:01 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char *arg)
{
	bool	n_flag;

	while (ft_isspace(*arg) == 1)
		arg++;
	if (*arg == '-' && arg[i + 1] == 'n' && ft_isspace(arg[i + 2] == 1))
		n_flag == true;
	if (arg)
		ft_printf("%s", arg);
	if (n_flag)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}
