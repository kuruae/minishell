/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@42.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:40:58 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/16 04:57:01 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	This function is used to imitate the real pwd call in bash.
*/

t_exec_error	ft_pwd(t_directory *dir, int fd_out)
{
	if (getcwd(dir->current_path, MAX_PATH) == NULL)
		ft_printf("%s", dir->current_path);
	else
		ft_putstr_fd(dir->current_path, fd_out);
	ft_putchar_fd('\n', fd_out);
	return (EXEC_SUCCESS);
}
