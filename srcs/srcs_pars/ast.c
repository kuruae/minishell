/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:04:19 by enzo              #+#    #+#             */
/*   Updated: 2024/12/10 19:50:31 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	paser_advance(t_parser *parser)
{
	if (parser->current->type != TOK_EOF)
		parser->current = parser->current->next;
}

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
            free(node);
            return (NULL);
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
    {
        free_ast(node);
        return (NULL);
    }

    return node;
}

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
    {
        free(node);
        return (NULL);
    }

    // Expect closing parenthesis
    if (!parser->current || parser->current->type != TOK_PAR_CLOSE)
    {
        free_ast(node);
        return (NULL);
    }
    paser_advance(parser);

    // Parse any redirections that might follow the subshell
    if (!parse_redir(parser, node))
    {
        free_ast(node);
        return (NULL);
    }

    return node;
}

int	count_args(t_token *token)
{
	int	count;

	count = 0;
	while (token && token->type == TOK_WORD)
	{
		count++;
		token = token->next;
	}
	return (count);
}

t_ast_node	*parse_pipe(t_parser *parser)
{
	t_ast_node	*left;
	t_ast_node	*node;

	left = parse_command(parser);
	while (parser->current && parser->current->type == TOK_PIPE)
	{
		paser_advance(parser);
		node = create_ast_node(NODE_PIPE);
		if (!node)
		{
			free_ast(left);
			return (NULL);
		}
		node->data.pipe.left = left;
		node->data.pipe.right = parse_command(parser);
		if (!node->data.pipe.right)
		{
			free_ast(node);
			return (NULL);
		}
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
        type = (parser->current->type == TOK_AND) ? NODE_AND : NODE_OR;
        paser_advance(parser);

        node = create_ast_node(type);
        if (!node)
        {
            free_ast(left);
            return NULL;
        }

        node->data.logical_op.left = left;
        node->data.logical_op.right = parse_pipe(parser);
        
        if (!node->data.logical_op.right)
        {
            free_ast(node);
            return NULL;
        }

        left = node;
    }

    return left;
}

void free_ast(t_ast_node *node)
{
	t_redir *redir;
	t_redir *next;
	
	if (!node)
		return ;
	redir = node->redirections;
	while (redir)
	{
		next = redir->next;
		free(redir->file);
		free(redir);
		redir = next;
	}

	switch (node->type)
	   {
        case NODE_COMMAND:
            free(node->data.command.command);
            if (node->data.command.args)
            {
                for (int i = 0; i < node->data.command.arg_count; i++)
                    free(node->data.command.args[i]);
                free(node->data.command.args);
            }
            break;
            
        case NODE_PIPE:
            free_ast(node->data.pipe.left);
            free_ast(node->data.pipe.right);
            break;
            
        case NODE_SUBSHELL:
            free_ast(node->data.subshell.command);
            break;
            
        case NODE_AND:
        case NODE_OR:
            free_ast(node->data.logical_op.left);
            free_ast(node->data.logical_op.right);
            break;
    }
    
    free(node);
}

static t_error  start_ast(t_parser *parser, t_ast_node **root)
{
    *root = parse_logic(parser);
    if (!*root)
        return (FAILURE);
    return (SUCCESS);
}

t_ast_node	*parse_tokens(t_token *tokens)
{
	t_parser parser;
	t_ast_node *root;

	parser.tokens = tokens;
	parser.current = tokens;
	parser.last_error = SUCCESS;

	if (start_ast(&parser, &root) == FAILURE)
        return (NULL);

	return (root);
}
