/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:07:48 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/09 20:45:20 by jbaumfal         ###   ########.fr       */
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

bool check_flag(char *arg)
{
	if (!(*arg == '-' && *(arg + 1) == 'n')) // checking if there is a -n flag
		return  (false);
	if (!(*arg + 2) || ft_isspace(*(arg + 2)) == 1) // n-falg is whitespace sensitive
		return (true);
	return (false);
}


int	ft_echo(char *arg, int fd_out)
{
	bool	n_flag;

	if (!arg)
		return(ft_putchar_fd('\n', fd_out), EXIT_SUCCESS);
	n_flag = check_flag(arg);
	if (n_flag)
		arg += 2;
	while (ft_isspace(*arg) == 1) // skipping the whitespace
		arg++;
	while (ft_isspace(*arg) == 1) // skipping the whitespace after the flag (like the real minishell)
		arg++;
	if (arg)
		echo_putstr_fd(arg, fd_out); // using the std_out because as i understand the echo only printd there and cant be send to pipes
	if (!n_flag) // only adding newline when there is no n_flag
		ft_putchar_fd('\n', fd_out);
// If in the argument there are spaces at the end they should not be printed
// The question is if we already print them before
	return (EXIT_SUCCESS);
}
