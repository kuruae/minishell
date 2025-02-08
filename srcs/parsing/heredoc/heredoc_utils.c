/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 17:35:28 by enzo              #+#    #+#             */
/*   Updated: 2025/02/08 17:16:07 by emagnani         ###   ########.fr       */
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

void	write_current_line(int fd, char *line, char **env, char *del)
{
	char	*expanded_line;

	if (is_expansion_on(del))
	{
		expanded_line = expand_env_vars(line, env);
		write(fd, expanded_line, ft_strlen(expanded_line));
		write(fd, "\n", 1);
		free(expanded_line);
	}
	else
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
}
