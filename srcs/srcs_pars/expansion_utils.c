/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 22:24:05 by enzo              #+#    #+#             */
/*   Updated: 2025/01/04 19:11:37 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/* Expand if:
*    1. No quotes
*    2. Inside double quotes only
*    3. Inside single quotes nested in double quotes
*  Don't expand if:
*    1. Inside single quotes
*    2. inside double quotes nested in single quotes 
*/
bool should_expand(t_quote_depth depth)
{
	return (depth.single_quotes != 1);
}

void    update_quote_depth(t_quote_depth *depth, char c)
{
	if (c == '\'')
	{
		if (depth->double_quotes == 1)
		{
			if (depth->single_quotes == 2)
				depth->double_quotes = 0;
			else
				depth->single_quotes = 2;
		}
		else
			depth->single_quotes = !depth->single_quotes;
	}
	else if (c == '"')
	{
		if (depth->single_quotes == 1)
		{
			if (depth->double_quotes == 2)
				depth->single_quotes = 0;
			else
				depth->double_quotes = 2;
		}
		else
			depth->double_quotes = !depth->double_quotes;
	}
}

void	join_to_result(char **result, void *to_join, t_join_type type)
{
	char	*temp;

	temp = *result;
	if (type == JOIN_STR)
		*result = ft_strjoin(*result, (char *)to_join);
	else if (type == JOIN_CHAR)
		*result = ft_strjoinch(*result, *(char *)to_join);
	free(temp);
}