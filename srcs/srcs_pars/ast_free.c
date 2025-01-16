/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:35:28 by emagnani          #+#    #+#             */
/*   Updated: 2025/01/16 21:38:59 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_command_node(t_ast_node *node)
{
	int	i;

	free(node->data.command.command);
	if (node->data.command.args)
	{
		i = 0;
		while (i < node->data.command.arg_count)
		{
			free(node->data.command.args[i]);
			i++;
		}
		free(node->data.command.args);
	}
}

static void	free_redir(t_ast_node *node)
{
	t_redir	*next;
	t_redir	*redir;

	redir = node->redirections;
	while (redir)
	{
		next = redir->next;
		if (redir->type == REDIR_HEREDOC)
			unlink(redir->file);
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
	{
		free_ast(node->data.subshell.command);
	}
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
