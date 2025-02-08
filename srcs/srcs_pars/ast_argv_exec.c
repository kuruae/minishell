/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_argv_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 00:56:57 by kuru              #+#    #+#             */
/*   Updated: 2025/02/08 02:29:29 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_argv_exec(char **argv, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static t_error	copy_command_to_argv(t_ast_node *node)
{
	node->u_data.s_command.argv_exec[0]
		= ft_strdup(node->u_data.s_command.command);
	if (!node->u_data.s_command.argv_exec[0])
	{
		free(node->u_data.s_command.argv_exec);
		return (ERR_MALLOC);
	}
	return (SUCCESS);
}

static t_error	copy_args_to_argv(t_ast_node *node, int i)
{
	while (i <= node->u_data.s_command.arg_count)
	{
		node->u_data.s_command.argv_exec[i]
			= ft_strdup(node->u_data.s_command.args[i - 1]);
		if (!node->u_data.s_command.argv_exec[i])
		{
			cleanup_argv_exec(node->u_data.s_command.argv_exec, i);
			return (ERR_MALLOC);
		}
		i++;
	}
	return (SUCCESS);
}

t_error	create_argv_exec(t_ast_node *node)
{
	int	i;

	if (node->u_data.s_command.argv_exec)
		cleanup_argv_exec(node->u_data.s_command.argv_exec,
			node->u_data.s_command.arg_count + 1);
	node->u_data.s_command.argv_exec = malloc(sizeof(char *)
			* (node->u_data.s_command.arg_count + 2));
	if (!node->u_data.s_command.argv_exec)
		return (ERR_MALLOC);
	if (copy_command_to_argv(node) != SUCCESS)
		return (ERR_MALLOC);
	i = 1;
	if (node->u_data.s_command.args)
		if (copy_args_to_argv(node, i) != SUCCESS)
			return (ERR_MALLOC);
	node->u_data.s_command.argv_exec
	[node->u_data.s_command.arg_count + 1] = NULL;
	return (SUCCESS);
}
