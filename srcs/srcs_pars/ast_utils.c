#include "minishell.h"

t_ast_node *create_ast_node(t_node_type type)
{
	t_ast_node *node;

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

t_redir	*create_redir_node(t_token *token, char *file)
{
	t_redir *redir;

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
		redir->type = REDIR_HEREDOC;
	else
	{
		free(redir);
		return (NULL);
	}
	redir->file = ft_strdup(file);
	redir->next = NULL;
	return (redir);
}