/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:04:19 by enzo              #+#    #+#             */
/*   Updated: 2024/12/11 18:14:19 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_redir(t_ast_node *node, t_redir *redir)
{
	t_redir *tmp;

	if (!node->redirections)
	{
		node->redirections = redir;
		return ;
	}
	tmp = node->redirections;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = redir;
}


/* parse_redir: Parses all redirections for a command or subshell
 * @parser: Current parser state
 * @node: The AST node to attach redirections to
 *
 * Processes all consecutive redirections like:
 * - Input redirection (<)
 * - Output redirection (>)
 * - Append redirection (>>)
 * - Here-document (<<)
 *
 * Returns: true on success, false on failure
 */
bool	parse_redir(t_parser *parser, t_ast_node *node)
{
	t_token *redir_token;
	t_redir *redir;

	while (parser->current && _parser_is_token_type_redir(parser->current->type))
	{
		redir_token = parser->current;
		paser_advance(parser);
		
		if (!parser->current || parser->current->type != TOK_WORD)
			return (false);

		redir = create_redir_node(redir_token, parser->current->value);
		if (!redir)
			return (false);
		add_redir(node, redir);
		paser_advance(parser);
	}
	return (true);
}



/* parse_subshell: Parses a subshell expression (commands within parentheses)
 * @parser: Current parser state
 *
 * Processing steps:
 * 1. Validates and consumes opening parenthesis
 * 2. Recursively parses the entire command sequence inside parentheses
 * 3. Validates and consumes closing parenthesis
 * 4. Handles any redirections that follow the subshell
 *
 * Returns: A new AST node of type NODE_SUBSHELL or NULL on failure
 */
t_ast_node *parse_subshell(t_parser *parser)
{
    t_ast_node *node;

    // Skip the opening parenthesis
    if (parser->current->type != TOK_PAR_OPEN)
        return (NULL);
    paser_advance(parser);

    node = create_ast_node(NODE_SUBSHELL);
    if (!node)
        return (NULL);

    // Parse everything inside the parentheses as a complete command
    node->data.subshell.command = parse_logic(parser);
    if (!node->data.subshell.command)
       return (err_free_and_return(parser, node));

    // Expect closing parenthesis
    if (!parser->current || parser->current->type != TOK_PAR_CLOSE)
        return (err_free_and_return(parser, node));
    paser_advance(parser);

    // Parse any redirections that might follow the subshell
    if (!parse_redir(parser, node))
        return (err_free_and_return(parser, node));

    return node;
}



/* parse_command: Parses a single command or subshell expression
 * @parser: Current parser state
 *
 * This function handles two cases:
 * 1. Subshell expressions starting with '(' 
 * 2. Regular commands with their arguments
 *
 * For regular commands:
 * - Stores the command name
 * - Collects all arguments
 * - Handles any redirections (>, <, >>, <<)
 *
 * Returns: A new AST node of type NODE_COMMAND or NULL on failure
 */
t_ast_node *parse_command(t_parser *parser)
{
    t_ast_node *node;
    int args_count;
    int i;

    // First check if we're starting a subshell
    if (parser->current->type == TOK_PAR_OPEN)
        return parse_subshell(parser);

    // Otherwise, parse as a regular command
    node = create_ast_node(NODE_COMMAND);
    if (!node)
        return (NULL);

    // Store command name and advance
    node->data.command.command = ft_strdup(parser->current->value);
    paser_advance(parser);
    
    // Count and process arguments
    args_count = count_args(parser->current);
    if (args_count > 0)
    {
        node->data.command.args = malloc(sizeof(char *) * (args_count + 1));
        if (!node->data.command.args)
        {
            free(node->data.command.command);
            return (err_free_and_return(parser, node));
        }
        
        i = 0;
        while (parser->current && parser->current->type == TOK_WORD && i < args_count)
        {
            node->data.command.args[i] = ft_strdup(parser->current->value);
            paser_advance(parser);
            i++;
        }
        node->data.command.args[i] = NULL;
        node->data.command.arg_count = args_count;
    }

    // Parse any redirections that follow the command
    if (!parse_redir(parser, node))
       return (err_free_and_return(parser, node));

    return node;
}



/* parse_pipe: Parses pipe sequences (cmd1 | cmd2 | cmd3)
 * @parser: Current parser state
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
 * Returns: Root node of the pipe sequence or NULL on failure
 */
t_ast_node	*parse_pipe(t_parser *parser)
{
	t_ast_node	*left;
	t_ast_node	*node;

	left = parse_command(parser);
	while (parser->current && parser->current->type == TOK_PIPE)
	{
		paser_advance(parser);

        if (!parser->current || parser->current->type == TOK_EOF)
            return (err_free_and_return(parser, left));

		node = create_ast_node(NODE_PIPE);
		if (!node)
            return (err_free_and_return(parser, left));
		node->data.pipe.left = left;
		node->data.pipe.right = parse_command(parser);
		if (!node->data.pipe.right)
            return (err_free_and_return(parser, node));
		left = node;
	}
	return (left);
}

static t_error testing_pointer_parse_pipe(t_parser *parser, t_ast_node **left)
{ 
    *left = parse_pipe(parser);
    if (!*left)
        return (FAILURE);
    return (SUCCESS);
}



/* parse_logic: Parses logical operators (&& and ||)
 * @parser: Current parser state
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
 * Returns: Root node of the logical operation tree or NULL on failure
 */
t_ast_node *parse_logic(t_parser *parser)
{
    t_ast_node *left;
    t_ast_node *node;
    t_node_type type;

    left = NULL;

    // First get the leftmost command or subshell
    if (testing_pointer_parse_pipe(parser, &left) == FAILURE)
        return (NULL);
    
    // Keep processing while we find logical operators
    while (parser->current && (parser->current->type == TOK_AND || 
           parser->current->type == TOK_OR))
    {
        if (parser->current->type == TOK_AND)
            type = NODE_AND;
        else
            type = NODE_OR;

        paser_advance(parser);

        node = create_ast_node(type);
        if (!node)
            return (err_free_and_return(parser, left));

        node->data.logical_op.left = left;
        node->data.logical_op.right = parse_pipe(parser);
        
        if (!node->data.logical_op.right)
            return (err_free_and_return(parser, node));

        left = node;
    }

    return left;
}

static t_error  start_ast(t_parser *parser, t_ast_node **root)
{
    *root = parse_logic(parser);
    if (!*root)
        return (FAILURE);
    return (SUCCESS);
}


/* parse_tokens: Entry point for parsing a token stream
 * @tokens: Linked list of lexical tokens
 *
 * Main parser function that:
 * 1. Initializes parser state
 * 2. Starts parsing from highest level (logical operations)
 * 3. Builds complete AST for the input command line
 *
 * Returns: Root node of the complete AST or NULL on failure
 */
t_ast_node	*parse_tokens(t_token *tokens)
{
	t_parser parser;
	t_ast_node *root;

	parser.tokens = tokens;
	parser.current = tokens;
	parser.err_status = SUCCESS;

	if (start_ast(&parser, &root) == FAILURE)
        return (NULL);

	return (root);
}
