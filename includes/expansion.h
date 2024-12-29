/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 02:36:00 by enzo              #+#    #+#             */
/*   Updated: 2024/12/29 01:41:03 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "minishell.h"

typedef struct quote_depth
{
	int	single_quotes;
	int	double_quotes;
}	t_quote_depth;

typedef enum e_join_type
{
	JOIN_STR,
	JOIN_CHAR
}	t_join_type;

bool should_expand(t_quote_depth depth);
char	*ft_strjoinch(char *s1, char c);
void    update_quote_depth(t_quote_depth *depth, char c);
t_error expand_command_args(t_ast_node *node, char **env);
void	join_to_result(char **result, void *to_join, t_join_type type);

#endif
