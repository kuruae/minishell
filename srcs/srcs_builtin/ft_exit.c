/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:25:20 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/06 14:36:10 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_is_num(char *str)
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

void	clean_exit(int i, t_shell *shell)
{
	clean_up_child(shell);
	exit(i);
}

t_exec_error	ft_exit(char **args, int argc, t_shell *shell)
{
	if (!args)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		clean_exit(0, shell);
	}
	if (argc > 1)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		ft_putstr_fd("total error: exit: too many arguments\n", STDERR_FILENO);
		clean_exit(1, shell);
	}
	if (!ft_is_num(args[0]))
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		ft_putstr_fd("exit: numeric argument required\n", STDERR_FILENO);
		g_sig_offset = 2;
		clean_exit(2, shell);
	}
	g_sig_offset = ft_atoi(args[0]);
	if (g_sig_offset < 0 || g_sig_offset > 255)
		g_sig_offset = g_sig_offset % 256;
	ft_putstr_fd("exit\n", STDERR_FILENO);
	clean_exit(g_sig_offset, shell);
	return (EXEC_SUCCESS);
}
