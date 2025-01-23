/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_expand_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:01:57 by emagnani          #+#    #+#             */
/*   Updated: 2025/01/23 00:23:06 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	has_dollar_expansion(t_ast_node *node)
{
	int	i;

	if (!node->data.command.command)
		return (false);
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
