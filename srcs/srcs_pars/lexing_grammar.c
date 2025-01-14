/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_grammar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:52:56 by emagnani          #+#    #+#             */
/*   Updated: 2025/01/14 19:48:00 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_error	op_grammar_handler(t_token *current, t_token *prev)
{
	if (!current->next || (current->next->type != TOK_WORD
		&& current->next->type != TOK_PAR_OPEN) || !prev)
		return (FAILURE);
	return (SUCCESS);
}

static t_error	quotes_grammar_handler(t_token *current)
{
	if (is_unclosed_quote(current->value))
		return (FAILURE);
	return (SUCCESS);
}

static t_error	redir_grammar_handler(t_token *current)
{
	if (!current->next || current->next->type != TOK_WORD)
		return (FAILURE);
	return (SUCCESS);
}

static t_error	subshell_grammar_handler(t_token *current, t_token *prev)
{
	if (!prev || prev->type != TOK_PAR_OPEN)
		return (FAILURE);
	return (SUCCESS);
}
{
	if (!current->next || current->next->type != TOK_PAR_CLOSE)
		return (FAILURE);
	return (SUCCESS);
}

t_error	grammar_handler(t_token *tokens)
{
	t_token	*current;
	t_token	*prev;
	t_error	error;

	error = SUCCESS;
	current = tokens;
	prev = NULL;
	error = is_subshell_valid(tokens);
	while (current && error == SUCCESS)
	{
		if (current->type == TOK_WORD)
			error = quotes_grammar_handler(current);
		else if (is_redir(current->type))
			error = redir_grammar_handler(current);
		else if (is_operator(current->type))
			error = op_grammar_handler(current, prev);
		else if (current->type == TOK_PAR_CLOSE)
			error = SUCCESS;
		prev = current;
		current = current->next;	
	}
	return (error);
}
