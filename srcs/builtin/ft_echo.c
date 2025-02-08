/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:07:48 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/07 20:18:43 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (ft_isprint(s[i]) && s[i] != '\\')
			write(fd, &s[i], 1);
		i++;
	}
}

/*
** This checks if here is a "-n" flag
** the flags can have multiple ns like "-nnnnnnnnnnnnnnn"
** As there can be multiple -nflags, the function returnsthe amunt of -n flags
*/

int	check_flag(char **args)
{
	int	i;
	int	j;

	i = 0;
	while (args[i] && args[i][0] && args[i][1])
	{
		j = 1;
		if (args[i][0] && args[i][1] && args[i][0] == '-' && args[i][1] == 'n')
		{
			while (args[i][j] && args[i][j] == 'n')
				j++;
			if (args[i][j] != '\0')
				break ;
		}
		else
			break ;
		i++;
	}
	return (i);
}

t_exec_error	ft_echo(char **args, int argc, int fd_out)
{
	int	n_flag;
	int	i;

	i = 0;
	if (argc == 0)
		return (ft_putchar_fd('\n', fd_out), EXEC_SUCCESS);
	n_flag = check_flag(args);
	i = i + n_flag;
	if (i < argc)
		echo_putstr_fd(args[i++], fd_out);
	while (i < argc)
	{
		ft_putchar_fd(' ', fd_out);
		echo_putstr_fd(args[i++], fd_out);
	}
	if (!n_flag)
		ft_putchar_fd('\n', fd_out);
	return (EXEC_SUCCESS);
}
