/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_subshell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 01:01:55 by kuru              #+#    #+#             */
/*   Updated: 2025/02/01 01:09:47 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/**
 * Parses a subshell expression (commands within parentheses)
 * 
 * @param parser Current parser state
 *
 * Processing steps:
 * 1. Validates and consumes opening parenthesis
 * 2. Recursively parses the entire command sequence inside parentheses
 * 3. Validates and consumes closing parenthesis
 * 4. Handles any redirections that follow the subshell
 *
 * @return A new AST node of type NODE_SUBSHELL or NULL on failure
 */
t_ast_node	*parse_subshell(t_parser *parser)
{
	t_ast_node	*node;

	if (parser->current->type != TOK_PAR_OPEN)
		return (NULL);
	paser_advance(parser);
	node = create_ast_node(NODE_SUBSHELL);
	if (!node)
		return (NULL);
	node->data.subshell.command = parse_logic(parser);
	if (!node->data.subshell.command)
		return (err_free_and_return(parser, node));
	if (!parser->current || parser->current->type != TOK_PAR_CLOSE)
		return (err_free_and_return(parser, node));
	paser_advance(parser);
	if (!parse_redir(parser, node))
		return (err_free_and_return(parser, node));
	return (node);
}
