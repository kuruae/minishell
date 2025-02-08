/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_token_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:52:09 by emagnani          #+#    #+#             */
/*   Updated: 2025/01/20 15:01:30 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(char *value, size_t len, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->expands = false;
	if (type == TOK_WORD && value[0] == '$' && len > 1)
		token->expands = true;
	token->value = ft_substr(value, 0, len);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_new_token(t_token **tokens, t_token *new_token)
{
	t_token	*tmp;

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
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}
