// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   run_commands.c                                     :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/10/10 17:56:25 by jbaumfal          #+#    #+#             */
// /*   Updated: 2024/12/03 15:24:10 by jbaumfal         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

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

	builtin_status = builtin(command, args, fd_out, shell->envp);
	if (builtin_status  == BI_ERR_MALLOC)
		return (ERR_MALLOC);
	//Checking with path


}
