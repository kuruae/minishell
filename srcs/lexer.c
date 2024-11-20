/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 23:56:06 by enzo              #+#    #+#             */
/*   Updated: 2024/11/20 16:42:50 by emagnani         ###   ########.fr       */
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
	}
	return (tokens);
}