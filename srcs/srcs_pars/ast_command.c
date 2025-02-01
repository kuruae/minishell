/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 00:43:04 by kuru              #+#    #+#             */
/*   Updated: 2025/02/01 01:18:57 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_error	init_empty_command(t_ast_node *node)
{
	node->data.command.command = ft_strdup("");
	if (!node->data.command.command)
		return (ERR_MALLOC);
	node->data.command.args = NULL;
	node->data.command.arg_count = 0;
	set_command_data(node);
	return (SUCCESS);
}

static t_error	process_single_arg(t_parser *parser, t_ast_node **node)
{
	char	**new_args;
	char	*new_arg;
	int		i;

	if (!parser->current || parser->current->type != TOK_WORD)
		return (SUCCESS);
	new_args = malloc(sizeof(char *) * ((*node)->data.command.arg_count + 2));
	if (!new_args)
		return (ERR_MALLOC);
	i = 0;
	while (i < (*node)->data.command.arg_count)
	{
		new_args[i] = (*node)->data.command.args[i];
		i++;
	}
	new_arg = ft_strdup(parser->current->value);
	if (!new_arg)
		return (free(new_args), ERR_MALLOC);
	new_args[i] = new_arg;
	new_args[i + 1] = NULL;
	free((*node)->data.command.args);
	(*node)->data.command.args = new_args;
	(*node)->data.command.arg_count++;
	paser_advance(parser);
	return (SUCCESS);
}

static t_error	handle_command_token(t_parser *parser,
	t_ast_node *node, bool *should_break)
{
	t_error	status;

	if (parser->current->type == TOK_WORD)
	{
		status = process_single_arg(parser, &node);
		if (status == ERR_FATAL)
			return (ERR_FATAL);
	}
	else if (_parser_is_token_type_redir(parser->current->type))
	{
		if (!parse_redir(parser, node) || g_sig_offset == 130)
			return (ERR_FATAL);
	}
	else
		*should_break = true;
	return (SUCCESS);
}

static t_error	process_command_tokens(t_parser *parser, t_ast_node *node)
{
	bool	should_break;
	t_error	status;

	should_break = false;
	while (parser->current && !should_break)
	{
		status = handle_command_token(parser, node, &should_break);
		if (status != SUCCESS)
			return (status);
	}
	return (SUCCESS);
}

/**
 * Parses a single command or subshell expression
 * 
 * @param parser Current parser state
 *
 * This function handles two cases:
 * 1. Subshell expressions starting with '(' 
 * 2. Regular commands with their arguments
 *
 * For regular commands:
 * - Stores the command name
 * - Collects all arguments
 * - Handles any redirections (>, <, >>, <<)
 * - Expands environment variables and wildcard patterns
 * - Creates an argv_exec array for execve
 * - Sets the command data structure
 * 
 * @return A new AST node of type NODE_COMMAND or NULL on failure
 */
t_ast_node	*parse_command(t_parser *parser)
{
	t_ast_node	*node;

	if (parser->current->type == TOK_PAR_OPEN)
		return (parse_subshell(parser));
	node = create_ast_node(NODE_COMMAND);
	if (!node)
		return (NULL);
	if (!parse_redir(parser, node) || g_sig_offset == 130)
		return (err_free_and_return(parser, node));
	if (!parser->current || parser->current->type != TOK_WORD)
	{
		if (init_empty_command(node) != SUCCESS)
			return (err_free_and_return(parser, node));
		return (node);
	}
	node->data.command.command = ft_strdup(parser->current->value);
	paser_advance(parser);
	if (process_command_tokens(parser, node) != SUCCESS)
		return (err_free_and_return(parser, node));
	if (remove_quotes_handler(node) != SUCCESS)
		return (err_free_and_return(parser, node));
	set_command_data(node);
	return (node);
}
