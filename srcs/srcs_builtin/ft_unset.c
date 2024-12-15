/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:45:13 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/15 17:49:39 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exec_error	remove_var(char	*var, char **envp)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(var);
	while (envp[i])
	{
		if (ft_strncmp(var, envp[i], len) == 0)
		{
			free(envp[i]);
			while (envp[i + 1])
			{
				envp[i] = ft_strdup(envp[i + 1]);
				if (!envp[i])
					return (EXEC_ERR_FATAL);
				free(envp[i + 1]);
				i++;
			}
			envp[i] = NULL;
			break ;
		}
		i++;
	}
	return (EXEC_SUCCESS);
}

t_exec_error	ft_unset(char **args, int argc, char **envp)
{
	int		i;

	i = 0;
	if (argc == 0)
		return (EXEC_SUCCESS);
	while (args[i])
	{
		if (remove_var(args[i++], envp) == EXEC_ERR_FATAL)
			return (EXEC_ERR_FATAL);
	}
	return (EXEC_SUCCESS);
}
