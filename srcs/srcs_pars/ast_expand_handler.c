/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_expand_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:01:57 by emagnani          #+#    #+#             */
/*   Updated: 2025/01/21 18:36:09 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	has_dollar_expansion(char **argv)
{
	int	i;

	if (!argv)
		return (false);
	i = 0;
	while (argv[i])
	{
		if (ft_strchr(argv[i], '$'))
			return (true);
		i++;
	}
	return (false);
}

// static bool	has_wildcard_expansion(char **argv)
// {
// 	int	i;

// 	if (!argv)
// 		return (false);
// 	i = 1;
// 	while (argv[i])
// 	{
// 		if (ft_strchr(argv[i], '*'))
// 			return (true);
// 		i++;
// 	}
// 	return (false);
// }

char	**all_expands_handler(char **argv, char **env)
{
	if (has_dollar_expansion(argv))
		return (start_dollar_expansion(argv, env));
	// if (has_wildcard_expansion(argv))
	// {
	// 	if (start_wildcard_expansion(argv) != SUCCESS)
	// 		return (FAILURE);
	// }
	return (argv);
}
