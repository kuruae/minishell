/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 01:02:05 by kuru              #+#    #+#             */
/*   Updated: 2025/02/07 20:35:15 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** 
 * Parses pipe sequences (cmd1 | cmd2 | cmd3)
 * 
 * @param parser Current parser state
 *
 * Creates a binary tree of pipe operations where:
 * - Left child is the command/subshell before the pipe
 * - Right child is the command/subshell after the pipe
 * - Handles multiple pipes by creating a left-associative tree
 *
 * Example for "cmd1 | cmd2 | cmd3":
 *        |
 *       / \
 *      |   cmd3
 *     / \
 *  cmd1  cmd2
 *
 * @return Root node of the pipe sequence or NULL on failure
 */
t_ast_node	*parse_pipe(t_parser *parser)
{
	t_ast_node	*left;
	t_ast_node	*node;

	left = parse_command(parser);
	if (!left)
		return (NULL);
	while (parser->current && parser->current->type == TOK_PIPE)
	{
		paser_advance(parser);
		if (!parser->current || parser->current->type == TOK_EOF)
			return (err_free_and_return(parser, left));
		node = create_ast_node(NODE_PIPE);
		if (!node)
			return (err_free_and_return(parser, left));
		node->u_data.s_pipe.left = left;
		node->u_data.s_pipe.right = parse_command(parser);
		if (parser->err_status == FAILURE)
			return (err_free_and_return(parser, node));
		if (!node->u_data.s_pipe.right)
			return (err_free_and_return(parser, node));
		left = node;
	}
	return (left);
}

t_error	pointer_parse_pipe(t_parser *parser, t_ast_node **left)
{
	*left = parse_pipe(parser);
	if (!*left && parser->err_status == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
