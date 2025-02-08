/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:09:16 by enzo              #+#    #+#             */
/*   Updated: 2025/02/08 16:54:33 by emagnani         ###   ########.fr       */
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
	ft_memset(&node->u_data, 0, sizeof(node->u_data));
	return (node);
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
	node->u_data.s_command.exec_data.in_type = STD_T;
	node->u_data.s_command.exec_data.out_type = STD_T;
	node->u_data.s_command.exec_data.in_file = STDIN_FILENO;
	node->u_data.s_command.exec_data.out_file = STDOUT_FILENO;
	node->u_data.s_command.exec_data.pipe_index_in = -1;
	node->u_data.s_command.exec_data.pipe_index_out = -1;
}
