/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 00:41:04 by kuru              #+#    #+#             */
/*   Updated: 2025/01/20 15:02:22 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_metacharacter(char *str)
{
	if (ft_strncmp(str, "&&", 2) == 0)
		return (true);
	if (str[0] == '|' || str[0] == '<' || str[0] == '>' || str[0] == '('
		|| str[0] == ')')
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

int	get_tok_word_len(char *line)
{
	int	len;
	int	i;

	len = ft_strlen(line);
	i = 0;
	while (i < len && !is_metacharacter(line + i) && !ft_isspace(line[i]))
	{
		if (line[i] == '\'' || line[i] == '\"')
			i += handle_quotes_len(line + i);
		i++;
	}
	return (i);
}
