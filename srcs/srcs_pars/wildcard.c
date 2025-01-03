// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   wildcard.c                                         :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/12/31 10:20:49 by enzo              #+#    #+#             */
// /*   Updated: 2024/12/31 11:34:24 by enzo             ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"

// t_error	expand_wildcard(t_ast_node *node, char **env)
// {
// 	char	**new_args;
// 	char	**expanded;
// 	int		total_args;
// 	int		i;
// 	int		j;
// 	int		k;

// 	total_args = 0;
// 	i = 0;
// 	while (i < node->data.command.arg_count)
// 	{
// 		expanded = expand_wildcard
// 	}
// }



// static int	count_matching_patterns(const char *pattern)
// {
// 	struct dirent	*entry;
// 	DIR				*dir;
// 	int				count;

// 	dir = opendir(".");
// 	if (!dir)
// 		return (0);

// 	count = 0;
// 	while ((entry = readdir(dir)))
// 	{
// 		if (entry->d_name[0] != '.' && pattern[0] != '.')
// 			break ;
// 	}
// }

// static void	bubble_sort_matches(char **matches, int count)
// {
// 	int		i;
// 	int		j;
// 	char	*tmp;

// 	i = 0;
// 	j = 0;
// 	while (i < count - 1)
// 	{
// 		while (j < count - (i - 1))
// 		{
// 			if (ft_strcmp(matches[j], matches[j + 1]) > 0)
// 			{
// 				tmp = matches[j];
// 				matches[j] = matches[j + 1];
// 				matches[j + 1] = tmp;
// 			}
// 			j++;
// 		}
// 		i++;
// 	}
// 	return (matches);
// }