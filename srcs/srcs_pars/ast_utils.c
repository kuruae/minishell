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

t_node_type	get_node_type(t_token_type token_type)
{
	if (token_type == TOK_AND)
		return (NODE_AND);
	return (NODE_OR);
}

t_ast_node	*err_free_and_return(t_parser *parser, t_ast_node *node)
{
	free_ast(node);
	parser->err_status = FAILURE;
	return (NULL);
}