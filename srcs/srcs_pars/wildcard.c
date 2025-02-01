/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:20:49 by enzo              #+#    #+#             */
/*   Updated: 2025/02/01 00:55:26 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_matching_patterns(const char *pattern)
{
	struct dirent	*entry;
	DIR				*dir;
	int				count;

	dir = opendir(".");
	if (!dir)
		return (0);
	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			continue ;
		if (is_pattern_matching(pattern, entry->d_name))
			count++;
	}
	closedir(dir);
	return (count);
}

static void	fill_matches(char **matches, const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				i;

	dir = opendir(".");
	if (!dir)
		return ;
	i = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			continue ;
		if (is_pattern_matching(pattern, entry->d_name))
			matches[i++] = ft_strdup(entry->d_name);
	}
	closedir(dir);
}

static char	**return_pattern_as_is(const char *pattern)
{
	char	**matches;

	matches = malloc(sizeof(char *) * 2);
	if (!matches)
		return (NULL);
	matches[0] = ft_strdup(pattern);
	if (!matches[0])
		return (free(matches), NULL);
	matches[1] = NULL;
	return (matches);
}

char	**expand_wildcard(const char *pattern)
{
	char	**matches;
	int		count;

	if (!ft_strchr(pattern, '*'))
		return (return_pattern_as_is(pattern));
	count = count_matching_patterns(pattern);
	if (count == 0)
		return (return_pattern_as_is(pattern));
	matches = malloc(sizeof(char *) * (count + 1));
	if (!matches)
		return (NULL);
	matches[count] = NULL;
	fill_matches(matches, pattern);
	matches = bubble_sort_matches(matches, count);
	return (matches);
}
