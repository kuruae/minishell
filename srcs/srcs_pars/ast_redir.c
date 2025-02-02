/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 00:43:44 by kuru              #+#    #+#             */
/*   Updated: 2025/02/02 21:45:52 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	_parser_is_token_type_redir(t_token_type type)
{
	return (type == TOK_REDIR_IN || type == TOK_REDIR_OUT
		|| type == TOK_APPEND || type == TOK_HEREDOC);
}

static t_redir	*init_redir_type(t_redir *redir, t_token *token,
									char *file, char **env)
{
	if (token->type == TOK_REDIR_IN)
		redir->type = REDIR_INPUT;
	else if (token->type == TOK_REDIR_OUT)
		redir->type = REDIR_OUTPUT;
	else if (token->type == TOK_APPEND)
		redir->type = REDIR_APPEND;
	else if (token->type == TOK_HEREDOC)
	{
		redir->type = REDIR_HEREDOC;
		redir->file = heredoc_handler(file, env);
		if (!redir->file)
			return (NULL);
	}
	else
		return (NULL);
	return (redir);
}

static t_redir	*create_redir_node(t_token *token, char *file, char **env)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	if (!init_redir_type(redir, token, file, env))
	{
		free(redir);
		return (NULL);
	}
	if (redir->type != REDIR_HEREDOC)
		redir->file = ft_strdup(file);
	redir->next = NULL;
	redir->head = redir;
	return (redir);
}

static void	add_redir(t_ast_node *node, t_redir *redir)
{
	t_redir	*tmp;
	t_redir	*head;

	if (!node->redirections)
	{
		node->redirections = redir;
		return ;
	}
	head = node->redirections;
	tmp = node->redirections;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = redir;
	redir->head = head;
}

/**
 * Parses all redirections for a command or subshell
 * 
 * @param parser	Current parser state
 * @param node		The AST node to attach redirections to
 *
 * Processes all consecutive redirections like:
 * - Input redirection (<)
 * - Output redirection (>)
 * - Append redirection (>>)
 * - Here-document (<<)
 *
 * @return true on success, false on failure
 */
bool	parse_redir(t_parser *parser, t_ast_node *node)
{
	t_token	*redir_token;
	t_redir	*redir;

	while (parser->current
		&& _parser_is_token_type_redir(parser->current->type))
	{
		redir_token = parser->current;
		paser_advance(parser);
		if (!parser->current || parser->current->type != TOK_WORD)
			return (false);
		redir = create_redir_node(redir_token,
				parser->current->value, parser->env);
		if (!redir)
			return (false);
		add_redir(node, redir);
		paser_advance(parser);
	}
	return (true);
}
