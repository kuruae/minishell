/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:35:28 by emagnani          #+#    #+#             */
/*   Updated: 2025/01/31 17:07:01 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_command_node(t_ast_node *node)
{
	int	i;

	if (!node)
		return ;
	if (node->data.command.command)
		free(node->data.command.command);
	if (node->data.command.args)
	{
		i = 0;
		while (i < node->data.command.arg_count)
		{
			if (node->data.command.args[i])
				free(node->data.command.args[i]);
			i++;
		}
		free(node->data.command.args);
	}
	if (node->data.command.argv_exec)
	{
		i = 0;
		while (node->data.command.argv_exec[i])
		{
			if (node->data.command.argv_exec[i])
				free(node->data.command.argv_exec[i]);
			i++;
		}
		free(node->data.command.argv_exec);
	}
}

static void	free_redir(t_ast_node *node)
{
	t_redir	*next;
	t_redir	*redir;

	if (!node)
		return ;
	redir = node->redirections;
	while (redir)
	{
		next = redir->next;
		if (redir->type == REDIR_HEREDOC)
			unlink(redir->file);
		if (redir->file)
			free(redir->file);
		free(redir);
		redir = next;
	}
}

void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	free_redir(node);
	if (node->type == NODE_COMMAND)
		free_command_node(node);
	else if (node->type == NODE_PIPE)
	{
		free_ast(node->data.pipe.left);
		free_ast(node->data.pipe.right);
	}
	else if (node->type == NODE_SUBSHELL)
		free_ast(node->data.subshell.command);
	else if (node->type == NODE_AND || node->type == NODE_OR)
	{
		free_ast(node->data.logical_op.left);
		free_ast(node->data.logical_op.right);
	}
	free(node);
}

t_ast_node	*err_free_and_return(t_parser *parser, t_ast_node *node)
{
	free_ast(node);
	parser->err_status = FAILURE;
	return (NULL);
}
