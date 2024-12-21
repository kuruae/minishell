/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tokens_and_ast.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 18:20:35 by emagnani          #+#    #+#             */
/*   Updated: 2024/12/21 17:30:21 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_lexing(t_token *tokens)
{
	t_token *tmp;

	if (!tokens)
		return ;
	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

void	free_user_input(t_token *tokens, t_ast_node *ast)
{
	free_lexing(tokens);
	free_ast(ast);
}