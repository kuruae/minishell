/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_grammar_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:31:55 by emagnani          #+#    #+#             */
/*   Updated: 2025/01/16 18:10:12 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_operator(t_token_type type)
{
	return (type == TOK_PIPE || type == TOK_AND || type == TOK_OR);
}

bool	is_redir(t_token_type type)
{
	return (type == TOK_REDIR_IN || type == TOK_REDIR_OUT
		|| type == TOK_APPEND || type == TOK_HEREDOC);
}

bool	is_unclosed_quote(char *str)
{
	int	i;
	int	single_quote;
	int	double_quote;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			single_quote += 1;
		if (str[i] == '\"')
			double_quote += 1;
		i++;
	}
	return (single_quote % 2 != 0 || double_quote % 2 != 0);
}

t_error	prev_and_after_par(t_token *current, t_token *prev)
{
	if (current->type == TOK_PAR_OPEN)
	{	
		if (prev && prev->type == TOK_WORD)
			return (FAILURE);
	}
	else if (current->type == TOK_PAR_CLOSE)
	{	
		if (current->next && current->next->type == TOK_WORD)
			return (FAILURE);
	}
	return (SUCCESS);
}

t_error	is_subshell_valid(t_token *tokens)
{
	t_token	*current;
	int		par_open;
	int		par_close;
	int		len;

	current = tokens;
	par_open = 0;
	par_close = 0;
	len = 0;
	while (current)
	{
		if (current->type == TOK_PAR_OPEN)
			par_open++;
		else if (current->type == TOK_PAR_CLOSE)
			par_close++;
		else if (par_open > 0)
			len++;
		if (par_close > par_open)
			return (FAILURE);
		current = current->next;
	}
	if (par_open != par_close || (len < 1 && par_open > 0))
		return (FAILURE);
	return (SUCCESS);
}
