/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_expand_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:01:57 by emagnani          #+#    #+#             */
/*   Updated: 2025/02/08 16:38:06 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	has_dollar_expansion(t_ast_node *node)
{
	int	i;

	if (!node->u_data.s_command.command)
		return (false);
	if (ft_strchr(node->u_data.s_command.command, '$'))
		return (true);
	if (node->u_data.s_command.args)
	{
		i = 0;
		while (node->u_data.s_command.args[i])
		{
			if (ft_strchr(node->u_data.s_command.args[i], '$'))
				return (true);
			i++;
		}
	}
	return (false);
}

static bool	has_wildcard_expansion(t_ast_node *node)
{
	int	i;

	if (node->u_data.s_command.args)
	{
		i = 0;
		while (node->u_data.s_command.args[i])
		{
			if (ft_strchr(node->u_data.s_command.args[i], '*'))
				return (true);
			i++;
		}
	}
	return (false);
}

t_error	all_expands_handler(t_ast_node *node, char **env)
{
	if (has_dollar_expansion(node))
	{
		if (start_dollar_expansion(node, env) != SUCCESS)
			return (FAILURE);
	}
	if (has_wildcard_expansion(node))
	{
		if (start_wildcard_expansion(node) != SUCCESS)
			return (FAILURE);
	}
	return (SUCCESS);
}
