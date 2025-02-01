/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 16:25:38 by emagnani          #+#    #+#             */
/*   Updated: 2025/02/01 00:54:52 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/**
 * Checks if a given name matches a pattern with wildcard support
 *
 * @param pattern	The pattern string, cam contain wildcards
 * @param name		The string to be matched against the pattern
 * 
 * algorithm:
 * 1. character by character comparison until '*' is found
 * 2. on '*' encounter:
 * 		- skip consecutive '*' characters
 * 		- if pattern ends with '*', rest of name matches
 * 		- otherwise, recursively try matching remaining
 * 			pattern at each position in the name
 * 3. of no '*', exact character matching is required
 *
 * * recursion:
 *   - each '*' spawns multiple recursive calls
 *   - each call tries matching remaining pattern at
 * 		different positions
 *   - returns true if any recursive path finds a match
 * 
 * @return true if the name matches the pattern, else false
 */
bool	is_pattern_matching(const char *pattern, const char *name)
{
	while (*pattern && *name)
	{
		if (*pattern == '*')
		{
			pattern++;
			while (*pattern == '*')
				pattern++;
			if (!*pattern)
				return (true);
			while (*name)
			{
				if (is_pattern_matching(pattern, name))
					return (true);
				name++;
			}
			return (is_pattern_matching(pattern, name));
		}
		else if (*pattern != *name)
			return (false);
		pattern++;
		name++;
	}
	while (*pattern == '*')
		pattern++;
	return (!*pattern && !*name);
}

char	**bubble_sort_matches(char **matches, int count)
{
	int		i;
	int		j;
	char	*tmp;

	if (!matches || count <= 0)
		return (matches);
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - 1 - i)
		{
			if (ft_strncmp(matches[j], matches[j + 1],
					ft_strlen(matches[j]) + 1) > 0)
			{
				tmp = matches[j];
				matches[j] = matches[j + 1];
				matches[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
	return (matches);
}
