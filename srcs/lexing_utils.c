/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 00:41:04 by kuru              #+#    #+#             */
/*   Updated: 2024/11/28 02:41:51 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_metacharacter(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '&') 
		return (true);
	return (false);
}

static int	handle_quotes_len(char *line)
{
	char	quote_type;
	int		i;
	
	i = 1;

	quote_type = line[0];
	while (line[i] && line[i] != quote_type)
		i++;
	return (i);
}

/* 
* @TODO: this part reminds me we will probably need to
* handle escape characters in the future (e.g. \', \$, etc.)
*/
int	get_tok_word_len(char *line)
{
	int len;
	int	i;

	len = ft_strlen(line);
	i = 0;
	while (i < len && !is_metacharacter(line[i]) && !ft_isspace(line[i]))
	{
		if (line[i] == '\'' || line[i] == '\"')
			i += handle_quotes_len(line + i);
		i++;
	}
	return (i);
}

t_token *create_token(char *value, size_t len, t_token_type type)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;
    token->value = ft_substr(value, 0, len);
    if (!token->value)
    {
        free(token);
        return NULL;
    }
    token->type = type;
    token->next = NULL;
    return token;
}

void	add_new_token(t_token **tokens, t_token *new_token)
{
	t_token *tmp;

	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	tmp = *tokens;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_token;
}

void	free_tokens(t_token *tokens)
{
	t_token *tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}