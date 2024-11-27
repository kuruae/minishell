/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 23:56:06 by enzo              #+#    #+#             */
/*   Updated: 2024/11/27 16:06:35 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *lexing(char *line)
{
	t_token *tokens;
	t_token *current;

	tokens = NULL;
	while(*line)
	{
		if (ft_isspace(*line))
			line++;
		else
			current = get_all_tokens_from_word(line);
	}
	return (tokens);
}

t_token	*get_all_tokens_from_word(char *line)
{
	int token_len;
	t_token_type token_type;

	token_type = get_token(line);
	token_len = get_token_len(line, token_type);
	return (create_token(line, token_len, token_type));
}

int	get_token_len(char *line, t_token_type type)
{
	len = 0;
	if (type == TOK_PIPE || type == TOK_REDIR_IN || type == TOK_REDIR_OUT)
	len = 1;
	else if (type == TOK_APPEND || type == TOK_HEREDOC || type == TOK_OR || type == TOK_AND)
		len = 2;
	else
		// vsy je suis fatigué je vois demain 
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
