/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 22:24:05 by enzo              #+#    #+#             */
/*   Updated: 2024/12/29 01:56:43 by enzo             ###   ########.fr       */
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

char	*ft_strjoinch(char *s1, char c)
{
	char	*result;
	int		i;

	if (!s1)
		return (NULL);
	result = malloc(sizeof(char) * (ft_strlen(s1) + 2));
	if (!result)
		return (NULL);
	i = 0;
	while (s1 && s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	result[i] = c;
	i++;
	result[i] = '\0';
	return (result);
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