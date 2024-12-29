/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 23:56:06 by enzo              #+#    #+#             */
/*   Updated: 2024/12/29 00:08:55 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token_type get_token_type(char *line)
{
	if (*line == '<')
	{
		if (ft_strncmp(line, "<<", 2) == 0)
			return (TOK_HEREDOC);
		return (TOK_REDIR_IN);
	}
	if (*line == '>')
	{
		if (ft_strncmp(line, ">>", 2) == 0)
			return (TOK_APPEND);
		return (TOK_REDIR_OUT);
	}
	if (*line == '|')
	{
		if (ft_strncmp(line, "||", 2) == 0)
			return (TOK_OR);
		return (TOK_PIPE);
	}
	if (ft_strncmp(line, "&&", 2) == 0)
			return (TOK_AND);
	if (*line == '(')
		return (TOK_PAR_OPEN);
	if (*line == ')')
		return (TOK_PAR_CLOSE);
	if (*line == '*')
		return (TOK_WILDCARD);
	else
		return (TOK_WORD);
}


size_t	get_token_len(char *line, t_token_type type)
{
	int len;

	if (type == TOK_PIPE || type == TOK_REDIR_IN || type == TOK_REDIR_OUT
				|| type == TOK_PAR_CLOSE || type == TOK_PAR_OPEN || type == TOK_WILDCARD)
		len = 1;
	else if (type == TOK_APPEND || type == TOK_HEREDOC || type == TOK_OR || type == TOK_AND)
		len = 2;
	else
		len = get_tok_word_len(line);
	return ((size_t)len);
}

t_token	*get_all_tokens_from_word(char *line)
{
	size_t		token_len;
	t_token_type token_type;

	token_type = get_token_type(line);
	token_len = get_token_len(line, token_type);
	return (create_token(line, token_len, token_type));
}

t_token *lexing(char *line)
{
	t_token *tokens;
	t_token *current;
	int 	i;

	tokens = NULL;
	i = 0;
	while(line[i])
	{
		if (ft_isspace(line[i]))
			i++;
		else
		{
			current = get_all_tokens_from_word(line + i);
			if (!current)
			{
				free_tokens(tokens);
				return (NULL);
			}
			add_new_token(&tokens, current);
			i += ft_strlen(current->value);
		}
	}
	if (verify_unclosed_quotes(tokens) != SUCCESS)
		return (NULL);
	return (tokens);
}


static bool	is_unclosed_quote(char *str)
{
	int i;
	int single_quote;
	int double_quote;

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

t_error	verify_unclosed_quotes(t_token *tokens)
{
	t_token *current;

	current = tokens;
	while (current)
	{
		if (current->type == TOK_WORD && is_unclosed_quote(current->value))
		{
			perror("syntax error: unclosed quote");
			free_lexing(tokens);
			return (FAILURE);
		}
		current = current->next;
	}
	return (SUCCESS);
}