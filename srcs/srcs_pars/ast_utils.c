/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:09:16 by enzo              #+#    #+#             */
/*   Updated: 2025/01/30 17:17:51 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*create_ast_node(t_node_type type)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->redirections = NULL;
	ft_memset(&node->data, 0, sizeof(node->data));
	return (node);
}

bool	_parser_is_token_type_redir(t_token_type type)
{
	return (type == TOK_REDIR_IN || type == TOK_REDIR_OUT
		|| type == TOK_APPEND || type == TOK_HEREDOC);
}

t_redir	*create_redir_node(t_token *token, char *file, char **env)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
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
		{
			free(redir);
			return (NULL);
		}
	}
	else
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

void	paser_advance(t_parser *parser)
{
	if (parser->current->type != TOK_EOF)
		parser->current = parser->current->next;
}

t_node_type	get_node_type(t_token_type token_type)
{
	if (token_type == TOK_AND)
		return (NODE_AND);
	return (NODE_OR);
}

void	set_command_data(t_ast_node *node)
{
	node->data.command.exec_data.in_type = STD_T;
	node->data.command.exec_data.out_type = STD_T;
	node->data.command.exec_data.in_file = STDIN_FILENO;
	node->data.command.exec_data.out_file = STDOUT_FILENO;
	node->data.command.exec_data.pipe_index_in = 0;
	node->data.command.exec_data.pipe_index_out = 0;
}
