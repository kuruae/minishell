/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 23:56:06 by enzo              #+#    #+#             */
/*   Updated: 2024/11/21 17:11:16 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *lexing(char *line)
{
	t_token *tokens;
	
	while(*line)
	{
		if (ft_isspace(*line))
			line++;
		else
			tokens->type = get_token(line);
	}
	return (tokens);
}

t_token_type get_token(char *line)
{
	if (*line == "<")
	{
		if (ft_strncmp(line, "<<", 2) == 0)
			return (TOK_HEREDOC);
		return (TOK_REDIR_IN);
	}
	if (*line == ">")
	{
		if (ft_strncmp(line, ">>", 2) == 0)
			return (TOK_APPEND);
		return (TOK_REDIR_OUT);
	}
	if (*line == "|")
	{
		if (ft_strncmp(line, "||", 2) == 0)
			return (TOK_OR);
		return (TOK_PIPE);
	}
	if (ft_strcmp(line, "&&") == 0)
		return (TOK_AND);
	else
		return (TOK_WORD);
}
