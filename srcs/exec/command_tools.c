/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@42.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 15:03:43 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/16 05:06:22 by jbaumfal         ###   ########.fr       */
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


/*
	This is the function that we call whenever we start the execution of a command
	- First we handle all the expansions in the command 
		(THis is is important as the expands can be changed during a command line (for example with an export command))
	- Then we handle the quotes (in the same way like the real bash shell)
	- The function create_argv_exec is used as prior the argv does not include the command itself
		- the execve funtion needs the command as the first argument
	- Then we check if the command is a directory (in that case we return an error)
	- We also check if the command is "neutral" (to ignore it)
		- special characters like "!" or "#" that arent a command in our bash but also doesnt return an error in the real bash
	- The last step of the preperation is to set the infile and outfile for the command
*/

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
