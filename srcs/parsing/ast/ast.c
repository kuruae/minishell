/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:04:19 by enzo              #+#    #+#             */
/*   Updated: 2025/02/01 18:21:22 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_error	start_ast(t_parser *parser, t_ast_node **root)
{
	*root = parse_logic(parser);
	if (!*root || parser->err_status == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

/**
 * Entry point for parsing a token stream
 * 
 * @param tokens Linked list of lexical tokens
 * @param env    Environment variables
 *
 * Main parser function that:
 * 1. Initializes parser state
 * 2. Starts parsing from highest level (logical operations)
 * 3. Builds complete AST for the input command line
 *
 * @return: Root node of the complete AST or NULL on failure
 */
t_ast_node	*ast_handler(t_token *tokens, char ***env)
{
	t_parser	parser;
	t_ast_node	*root;

	parser.tokens = tokens;
	parser.current = tokens;
	parser.env = *env;
	parser.err_status = SUCCESS;
	if (start_ast(&parser, &root) == FAILURE)
		return (NULL);
	free_tokens(parser.current);
	return (root);
}
