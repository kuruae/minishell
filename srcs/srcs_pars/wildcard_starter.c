/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_starter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 00:31:46 by kuru              #+#    #+#             */
/*   Updated: 2025/02/01 00:31:53 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_args(char **args, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(args[i]);
		i++;
	}
	free(args);
}

static void	cleanup_expanded(char **expanded)
{
	int	i;

	i = 0;
	while (expanded[i])
	{
		free(expanded[i]);
		i++;
	}
	free(expanded);
}

static int	count_expanded_args(t_ast_node *node, int *total)
{
	char	**expanded;
	int		i;
	int		j;

	*total = 0;
	i = 0;
	while (i < node->data.command.arg_count)
	{
		expanded = expand_wildcard(node->data.command.args[i]);
		if (!expanded)
			return (FAILURE);
		j = 0;
		while (expanded[j])
		{
			(*total)++;
			j++;
		}
		cleanup_expanded(expanded);
		i++;
	}
	return (SUCCESS);
}

static int	expand_and_fill(t_ast_node *node, char **new_args, int *k)
{
	char	**expanded;
	int		i;
	int		j;

	i = 0;
	*k = 0;
	while (i < node->data.command.arg_count)
	{
		expanded = expand_wildcard(node->data.command.args[i]);
		if (!expanded)
			return (FAILURE);
		j = 0;
		while (expanded[j])
		{
			new_args[*k] = ft_strdup(expanded[j]);
			(*k)++;
			j++;
		}
		cleanup_expanded(expanded);
		i++;
	}
	new_args[*k] = NULL;
	return (SUCCESS);
}

t_error	start_wildcard_expansion(t_ast_node *node)
{
	char	**new_args;
	int		total_args;
	int		k;

	if (count_expanded_args(node, &total_args) == FAILURE)
		return (FAILURE);
	new_args = malloc(sizeof(char *) * (total_args + 1));
	if (!new_args)
		return (ERR_MALLOC);
	if (expand_and_fill(node, new_args, &k) == FAILURE)
	{
		cleanup_args(new_args, k);
		return (FAILURE);
	}
	cleanup_args(node->data.command.args, node->data.command.arg_count);
	node->data.command.args = new_args;
	node->data.command.arg_count = total_args;
	return (SUCCESS);
}
