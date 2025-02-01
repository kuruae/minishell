/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:16:31 by enzo              #+#    #+#             */
/*   Updated: 2025/02/01 00:55:03 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARD_H
# define WILDCARD_H

# include "minishell.h"
# include <dirent.h>


t_error	start_wildcard_expansion(t_ast_node *node);
char	**bubble_sort_matches(char **matches, int count);
bool	is_pattern_matching(const char *pattern, const char *name);
char	**expand_wildcard(const char *pattern);

#endif