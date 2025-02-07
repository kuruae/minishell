/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 19:02:15 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/07 20:35:15 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void message_command_not_found(t_ast_node *node)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd(node->u_data.s_command.command, STDERR_FILENO);
    ft_putstr_fd(": command not found\n", STDERR_FILENO);
}