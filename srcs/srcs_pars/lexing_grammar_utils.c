/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_grammar_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:31:55 by emagnani          #+#    #+#             */
/*   Updated: 2025/02/05 19:38:02 by emagnani         ###   ########.fr       */
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
