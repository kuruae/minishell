/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:40:58 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/18 02:49:52 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exec_error	ft_pwd(t_directory *dir)
{
	if (getcwd(dir->current_path, MAX_PATH) == NULL)
		return (perror("pwd error"), EXEC_ERR_NON_FATAL);
	else
		ft_putstr_fd(dir->current_path, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXEC_SUCCESS);
}
