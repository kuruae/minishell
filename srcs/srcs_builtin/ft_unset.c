/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:45:13 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/08 16:41:39 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_var(char	*var, char **envp)
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
				free(envp[i + 1]);
				i++;
			}
			envp[i] = NULL;
			break ;
		}
		i++;
	}

}


int	ft_unset(char *arg,char **envp)
{
	char	**variables;
	int		i;

	i = 0;
	if (!arg)
		return (EXIT_SUCCESS);
	variables = ft_split(arg, ' ');
	if (!variables)
		return (free_all(variables), EXIT_FAILURE); // i dont know how we handle errors like this when mallocs dont work
	while (variables[i])
		remove_var(variables[i++], envp);
	ft_env(envp, 1);
	return (free_all(variables), EXIT_SUCCESS);
}
