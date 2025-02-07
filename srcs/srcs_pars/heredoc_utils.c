/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 17:35:28 by enzo              #+#    #+#             */
/*   Updated: 2025/02/07 19:25:52 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_expansion_on(char *delemiter)
{
	if (!delemiter)
		return (true);
	if (delemiter[0] == '\'' && delemiter[ft_strlen(delemiter) - 1] == '\'')
		return (false);
	return (true);
}

bool	check_delim_match(char *input, char *delimiter)
{
	char	*clean_delim;
	bool	match;

	clean_delim = remove_quotes_from_string(delimiter);
	match = (ft_strcmp(input, clean_delim) == 0);
	free(clean_delim);
	return (match);
}
