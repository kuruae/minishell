/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:07:48 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/06 15:42:43 by jbaumfal         ###   ########.fr       */
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

bool	check_flag(char **args)
{
	if (ft_strcmp(args[0], "-n") == 0)
		return (true);
	else
		return (false);
}

t_exec_error	ft_echo(char **args, int argc, int fd_out)
{
	bool	n_flag;
	int		i;

	i = 0;
	if (argc == 0)
		return (ft_putchar_fd('\n', fd_out), EXEC_SUCCESS);
	n_flag = check_flag(args);
	if (n_flag == true)
		i++;
	while ( i < argc && ft_strcmp(args[i], "-n") == 0)
		i++;
	if (i < argc)
		ft_putstr_fd(args[i++], fd_out);
	while (i < argc)
	{
		ft_putchar_fd(' ', fd_out);
		ft_putstr_fd(args[i++], fd_out);
	}
	if (!n_flag)
		ft_putchar_fd('\n', fd_out);
	return (EXEC_SUCCESS);
}
