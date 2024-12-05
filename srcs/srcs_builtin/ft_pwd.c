/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:40:58 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/05 14:21:17 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_directory *dir, int fd_out)
{
	if (getcwd(dir->current_path, MAX_PATH) == NULL)
		return (perror("pwd error"), EXIT_FAILURE);
	else
		ft_putstr_fd(dir->current_path, fd_out);
	ft_printf("\n");
	return (EXIT_SUCCESS);
}
