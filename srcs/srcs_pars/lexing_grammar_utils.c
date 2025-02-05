/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_grammar_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:31:55 by emagnani          #+#    #+#             */
/*   Updated: 2025/02/05 22:55:51 by kuru             ###   ########.fr       */
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
    t_quote_depth	depth;
    int		        i;

    depth.single_quotes = 0;
    depth.double_quotes = 0;
    i = 0;
    while (str[i])
    {
        if (str[i] == '\'' && !depth.double_quotes)
            depth.single_quotes = !depth.single_quotes;
        else if (str[i] == '\"' && !depth.single_quotes)
            depth.double_quotes = !depth.double_quotes;
        i++;
    }
    return (depth.single_quotes || depth.double_quotes);
}
