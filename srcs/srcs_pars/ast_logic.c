/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_logic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 00:43:22 by kuru              #+#    #+#             */
/*   Updated: 2025/02/01 01:16:33 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Parses logical operators (&& and ||)
 * 
 * @param parser Current parser state
 *
 * Handles logical AND (&&) and OR (||) operations by:
 * 1. First parsing the leftmost pipe sequence or command
 * 2. While logical operators are found:
 *    - Creates a new logical operation node
 *    - Sets left child to previous command/operation
 *    - Sets right child to next pipe sequence
 *
 * Example for "cmd1 && cmd2 || cmd3":
 *         ||
 *        /  \
 *      &&   cmd3
 *     /  \
 *  cmd1  cmd2
 *
 * @return Root node of the logical operation tree or NULL on failure
 */
t_ast_node	*parse_logic(t_parser *parser)
{
	t_ast_node	*left;
	t_ast_node	*node;
	t_node_type	type;

	left = NULL;
	if (pointer_parse_pipe(parser, &left) == FAILURE)
		return (NULL);
	while (parser->current && (parser->current->type == TOK_AND
			|| parser->current->type == TOK_OR))
	{
		type = get_node_type(parser->current->type);
		paser_advance(parser);
		node = create_ast_node(type);
		if (!node)
			return (err_free_and_return(parser, left));
		node->data.logical_op.left = left;
		node->data.logical_op.right = parse_pipe(parser);
		if (parser->err_status == FAILURE)
			return (err_free_and_return(parser, node));
		left = node;
	}
	return (left);
}
