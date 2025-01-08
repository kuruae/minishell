/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 02:36:00 by enzo              #+#    #+#             */
/*   Updated: 2025/01/08 18:35:11 by emagnani         ###   ########.fr       */
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

bool	should_expand(t_quote_depth depth);
char	*ft_strjoinch(char *s1, char c);
void    update_quote_depth(t_quote_depth *depth, char c);
t_error	start_dollar_expansion(t_ast_node *node, char **env);

#endif
