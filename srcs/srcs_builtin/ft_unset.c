/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:45:13 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/07 14:01:14 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_var(char	*var, char **envp)
{
	char	*env_line;
	int		i;
	size_t	len;

	i = 0;
	env_line = getenv(var);
	len = ft_strlen(var);
	ft_printf("env_line: %s\n", env_line);
	if (!env_line)
		return ;
	while (envp[i])
	{
		if (ft_strncmp(var, envp[i], len) == 0)
		{
			free(envp[i]);
			while (envp[i + 1])
			{
				envp[i] = envp[i + 1];
				free(envp[i]);
				i++;
			}
		}
		i++;
	}

}


int	ft_unset(char *arg,char **envp)
{
	char	**variables;
	int		i;

	i = 0;
	variables = ft_split(arg, ' ');
	if (!variables)
		return (free_all(variables), EXIT_FAILURE); // i dont know how we handle errors like this when mallocs dont work
	while (variables[i])
		remove_var(variables[i++], envp);
	return (free_all(variables), EXIT_SUCCESS);
}
