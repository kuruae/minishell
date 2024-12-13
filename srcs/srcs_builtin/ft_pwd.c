/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:40:58 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/13 21:36:41 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bi_error	ft_pwd(t_directory *dir, int fd_out)
{
	if (getcwd(dir->current_path, MAX_PATH) == NULL)
		return (perror("pwd error"), BI_ERR_NON_FATAL);
	else
		ft_putstr_fd(dir->current_path, fd_out);
	ft_putchar_fd('\n', fd_out);
	return (BI_SUCCESS);
}
