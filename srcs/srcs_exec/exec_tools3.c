/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 15:32:08 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/05 15:45:49 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	When someone manually removes the PATH variable from the environment
	This function would return NULL
	However this would it make it impossible for me to know,
    if a malloc failed or the PATH is not there
	Thats why I return the string "null" in case of a missing PATH variable 
*/
char	**get_paths(char **env)
{
	int		i;
	char	*path_value;
	char	**paths;

	i = 0;
	path_value = NULL;
	if (!env || !*env)
		return (NULL);
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path_value = ft_strdup(env[i] + 5);
			if (!path_value)
				return (NULL);
			break ;
		}
		i++;
	}
	if (!path_value)
		return (ft_split(ft_strdup("null"), ':'));
	paths = ft_split(path_value, ':');
	return (free(path_value), paths);
}
