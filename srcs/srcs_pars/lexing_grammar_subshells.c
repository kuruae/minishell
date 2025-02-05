/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_grammar_subshells.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 19:37:32 by emagnani          #+#    #+#             */
/*   Updated: 2025/02/05 19:56:32 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static bool	out_of_subshell_pipe(t_token_type type, int par_open, int par_close)
{
	if (par_close >= par_open && par_open > 0)
	{		
		if (type == TOK_PIPE)
			return (false);
	}
	return (true);
}

t_error	is_subshell_valid(t_token *tokens)
{
	t_token			*current;
	t_par_counts	counts;
	int				len;

	current = tokens;
	counts = (t_par_counts){0, 0};
	len = 0;
	while (current)
	{
		if (current->type == TOK_PAR_OPEN)
			counts.open++;
		else if (current->type == TOK_PAR_CLOSE)
			counts.close++;
		else if (counts.open > 0)
			len++;
		if (counts.close > counts.open)
			return (FAILURE);
		if (!out_of_subshell_pipe(current->type, counts.open, counts.close))
			return (FAILURE);
		current = current->next;
	}
	if (counts.open != counts.close || (len < 1 && counts.open > 0))
		return (FAILURE);
	return (SUCCESS);
}
