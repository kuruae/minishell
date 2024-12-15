/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:28:49 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/15 01:48:32 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


t_exec_error exec_command(t_shell *shell, t_ast_node *node, int fd_out)
{
	char		*command;
	char		**args;
	int			arg_count;
	t_bi_error	builtin_status;

	command = node->data.command.command;
	args = node->data.command.args;
	arg_count = node->data.command.arg_count;

	builtin_status = builtin(command, args, arg_count, fd_out, shell->envp);
	if (builtin_status  == BI_ERR_MALLOC)
		return (ERR_MALLOC);
	//Checking with path

	return (EXEC_SUCCESS);
}
