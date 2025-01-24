/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_start.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 17:55:45 by emagnani          #+#    #+#             */
/*   Updated: 2025/01/24 17:55:56 by emagnani         ###   ########.fr       */
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

	expanded = expand_env_vars(node->data.command.command, env);
	if (!expanded)
		return (ERR_MALLOC);
	free(node->data.command.command);
	node->data.command.command = expanded;
	if (node->data.command.args)
	{
		i = 0;
		while (i < node->data.command.arg_count)
		{
			expanded = expand_env_vars(node->data.command.args[i], env);
			if (!expanded)
				return (ERR_MALLOC);
			free(node->data.command.args[i]);
			node->data.command.args[i] = expanded;
			i++;
		}
	}
	return (SUCCESS);
}
