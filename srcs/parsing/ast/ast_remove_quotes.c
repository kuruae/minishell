/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_remove_quotes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:05:23 by enzo              #+#    #+#             */
/*   Updated: 2025/02/08 16:54:11 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_quote_if_needed(char *str, int i, t_quote_depth *state)
{
	if (!state)
		return (i);
	if (str[i] == '\'' && state->double_quotes == 0)
	{
		state->single_quotes = !state->single_quotes;
		i++;
	}
	else if (str[i] == '\"' && state->single_quotes == 0)
	{
		state->double_quotes = !state->double_quotes;
		i++;
	}
	else
		i++;
	return (i);
}

char	*remove_quotes_from_string(char *str)
{
	int				i;
	int				j;
	char			*quoteless;
	t_quote_depth	state;

	quoteless = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!quoteless)
		return (NULL);
	i = 0;
	j = 0;
	state = (t_quote_depth){0, 0};
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if ((str[i] == '\'' && state.double_quotes)
				|| (str[i] == '\"' && state.single_quotes))
				quoteless[j++] = str[i];
			i = skip_quote_if_needed(str, i, &state);
			continue ;
		}
		quoteless[j++] = str[i++];
	}
	quoteless[j] = '\0';
	return (quoteless);
}

t_error	remove_quotes_handler(t_ast_node *node)
{
	int		i;
	char	*quoteless;

	quoteless = remove_quotes_from_string(node->u_data.s_command.command);
	if (!quoteless)
		return (ERR_MALLOC);
	free(node->u_data.s_command.command);
	node->u_data.s_command.command = quoteless;
	if (node->u_data.s_command.args)
	{
		i = 0;
		while (i < node->u_data.s_command.arg_count)
		{
			quoteless
				= remove_quotes_from_string(node->u_data.s_command.args[i]);
			if (!quoteless)
				return (ERR_MALLOC);
			free(node->u_data.s_command.args[i]);
			node->u_data.s_command.args[i] = quoteless;
			i++;
		}
	}
	return (SUCCESS);
}
