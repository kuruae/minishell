/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_expand_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:01:57 by emagnani          #+#    #+#             */
/*   Updated: 2025/01/13 16:54:32 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	has_dollar_expansion(t_ast_node *node)
{
	int	i;

	if (ft_strchr(node->data.command.command, '$'))
		return (true);
	if (node->data.command.args)
	{
		i = 0;
		while (node->data.command.args[i])
		{
			if (ft_strchr(node->data.command.args[i], '$'))
				return (true);
			i++;
		}
	}
	return (false);
}

static bool	has_wildcard_expansion(t_ast_node *node)
{
	int	i;

	if (node->data.command.args)
	{
		i = 0;
		while (node->data.command.args[i])
		{
			if (ft_strchr(node->data.command.args[i], '*'))
				return (true);
			i++;
		}
	}
	return (false);
}

t_error	all_expands_handler(t_ast_node *node, t_parser *parser)
{
	if (has_dollar_expansion(node))
	{
		if (start_dollar_expansion(node, parser->env) != SUCCESS)
			return (FAILURE);
	}
	if (has_wildcard_expansion(node))
	{
		if (start_wildcard_expansion(node) != SUCCESS)
			return (FAILURE);
	}
	return (SUCCESS);
}
