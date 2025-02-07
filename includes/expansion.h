/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 02:36:00 by enzo              #+#    #+#             */
/*   Updated: 2025/02/07 21:20:26 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "ast.h"

typedef struct quote_depth
{
	int	single_quotes;
	int	double_quotes;
}	t_quote_depth;

bool		should_expand(t_quote_depth depth);
char		*ft_strjoinch(char *s1, char c);
void		update_quote_depth(t_quote_depth *depth, char c);
t_error		start_dollar_expansion(t_ast_node *node, char **env);
char		*expand_env_vars(const char *str, char **env);

#endif
