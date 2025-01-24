/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:20:49 by enzo              #+#    #+#             */
/*   Updated: 2025/01/23 00:44:38 by kuru             ###   ########.fr       */
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
static bool	is_pattern_matching(const char *pattern, const char *name)
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

static int	count_matching_patterns(const char *pattern)
{
	struct dirent	*entry;
	DIR				*dir;
	int				count;

	dir = opendir(".");
	if (!dir)
		return (0);
	count = 0;
	while ((entry = readdir(dir)))
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
	while ((entry = readdir(dir)))
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			continue ;
		if (is_pattern_matching(pattern, entry->d_name))
			matches[i++] = ft_strdup(entry->d_name);
	}
	closedir(dir);
}

static char	**bubble_sort_matches(char **matches, int count)
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

static char	**expand_wildcard(const char *pattern)
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

t_error	start_wildcard_expansion(t_ast_node *node)
{
	   char    **new_args;
	char    **expanded;
	int     total_args;
	int     i;
	int     j;
	int     k;

	printf("Expanding wildcard signs reached!\n");
	// Count total arguments after expansion
	total_args = 0;
	for (i = 0; i < node->data.command.arg_count; i++)
	{
		expanded = expand_wildcard(node->data.command.args[i]);
		if (!expanded)
			return (FAILURE);
			
		for (j = 0; expanded[j]; j++)
			total_args++;
			
		// Free the expansion (we're just counting for now)
		for (j = 0; expanded[j]; j++)
			free(expanded[j]);
		free(expanded);
	}

	// Allocate new argument array
	new_args = malloc(sizeof(char *) * (total_args + 1));
	if (!new_args)
		return (ERR_MALLOC);

	// Fill new argument array
	k = 0;
	for (i = 0; i < node->data.command.arg_count; i++)
	{
		expanded = expand_wildcard(node->data.command.args[i]);
		if (!expanded)
		{
			free(new_args);
			return (FAILURE);
		}
		
		for (j = 0; expanded[j]; j++)
			new_args[k++] = ft_strdup(expanded[j]);
			
		// Free the expansion
		for (j = 0; expanded[j]; j++)
			free(expanded[j]);
		free(expanded);
	}
	new_args[k] = NULL;

	// Free old arguments
	for (i = 0; i < node->data.command.arg_count; i++)
		free(node->data.command.args[i]);
	free(node->data.command.args);

	// Update node
	node->data.command.args = new_args;
	node->data.command.arg_count = total_args;

	return (SUCCESS);
}
