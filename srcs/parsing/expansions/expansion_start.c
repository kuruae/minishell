/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_start.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 17:55:45 by emagnani          #+#    #+#             */
/*   Updated: 2025/02/08 16:54:47 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/**
 * Start the dollar expansion process for a command node
 * 
 * @param node	the command node to expand
 * @param env	the environment variables
 * 
 * expand the command name and all arguments
 * 
 * @return SUCCESS or ERR_MALLOC
 */
t_error	start_dollar_expansion(t_ast_node *node, char **env)
{
	char	*expanded;
	int		i;

	expanded = expand_env_vars(node->u_data.s_command.command, env);
	if (!expanded)
		return (ERR_MALLOC);
	free(node->u_data.s_command.command);
	node->u_data.s_command.command = expanded;
	if (node->u_data.s_command.args)
	{
		i = 0;
		while (i < node->u_data.s_command.arg_count)
		{
			expanded = expand_env_vars(node->u_data.s_command.args[i], env);
			if (!expanded)
				return (ERR_MALLOC);
			free(node->u_data.s_command.args[i]);
			node->u_data.s_command.args[i] = expanded;
			i++;
		}
	}
	return (SUCCESS);
}
