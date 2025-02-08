/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:35:28 by emagnani          #+#    #+#             */
/*   Updated: 2025/02/08 16:53:50 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_command_node(t_ast_node *node)
{
	int	i;

	if (node->u_data.s_command.command)
		free(node->u_data.s_command.command);
	if (node->u_data.s_command.args)
	{
		i = 0;
		while (i < node->u_data.s_command.arg_count)
		{
			if (node->u_data.s_command.args[i])
				free(node->u_data.s_command.args[i]);
			i++;
		}
		free(node->u_data.s_command.args);
	}
	if (node->u_data.s_command.argv_exec)
	{
		i = 0;
		while (node->u_data.s_command.argv_exec[i])
		{
			free(node->u_data.s_command.argv_exec[i]);
			i++;
		}
		free(node->u_data.s_command.argv_exec);
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
		if (redir->e_type == REDIR_HEREDOC)
			unlink(redir->file);
		if (redir->file)
			free(redir->file);
		free(redir);
		redir = next;
	}
}

/**
 * @brief Recursively frees an AST node and its children
 *
 * This function handles different types of nodes:
 * - Command nodes: frees command-specific data
 * - Pipe nodes: frees both left and right branches
 * - Subshell nodes: frees the contained command
 * - Logical nodes (AND/OR): frees both left and right operands
 * 
 * @param node Pointer to the AST node to be freed
 * 
 * The function first frees any redirections associated with the node,
 * then processes the node according to its type, and finally frees
 * the node itself.
 */
void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	free_redir(node);
	if (node->type == NODE_COMMAND)
		free_command_node(node);
	else if (node->type == NODE_PIPE)
	{
		free_ast(node->u_data.s_pipe.left);
		free_ast(node->u_data.s_pipe.right);
	}
	else if (node->type == NODE_SUBSHELL)
		free_ast(node->u_data.s_subshell.command);
	else if (node->type == NODE_AND || node->type == NODE_OR)
	{
		free_ast(node->u_data.s_logical_op.left);
		free_ast(node->u_data.s_logical_op.right);
	}
	free(node);
}

t_ast_node	*err_free_and_return(t_parser *parser, t_ast_node *node)
{
	free_ast(node);
	parser->err_status = FAILURE;
	return (NULL);
}
