/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 15:03:43 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/08 17:50:49 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_directory(char *path)
{
	int	i;

	if (!path || !*path)
		return (false);
	i = 0;
	while (path[i + 1])
		i++;
	if (path[i] == '/')
	{
		g_sig_offset = 126;
		return (true);
	}
	else if (path[i] == '.' && path[i - 1] == '/')
	{
		g_sig_offset = 126;
		return (true);
	}
	else if (path[i] == '.' && path[i - 1] == '.' && path[i - 2] == '/')
	{
		g_sig_offset = 126;
		return (true);
	}
	else
		return (false);
}

bool	is_neutral_char(t_ast_node *node)
{
	char	*command;

	command = node->u_data.s_command.command;
	if (ft_strlen(command) == 1)
	{
		if (command[0] == '!' || command[0] == '#' || command[0] == ':')
			return (true);
	}
	return (false);
}

t_exec_error	prepare_command(t_shell *shell, t_ast_node *node)
{
	if (all_expands_handler(node, *shell->envp) != SUCCESS)
		return (EXEC_ERR_FATAL);
	if (remove_quotes_handler(node) != SUCCESS)
		return (EXEC_ERR_FATAL);
	if (create_argv_exec(node) != SUCCESS)
		return (EXEC_ERR_FATAL);
	if (is_directory(node->u_data.s_command.command) == true)
	{
		ft_putstr_fd("total error: is a directory\n", 2);
		return (EXEC_NOT_FOUND);
	}
	if (is_neutral_char(node) == true)
	{
		g_sig_offset = 0;
		if (shell->pipeline == true)
			shell->process_count--;
		close_redirections(node);
		return (EXEC_NEUTRAL);
	}
	if (set_infile_outfile(shell, node) == EXEC_ERR_FILE)
		return (set_sig_offset(EXEC_ERR_FILE), EXEC_ERR_FILE);
	return (EXEC_SUCCESS);
}
