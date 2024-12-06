/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:45:13 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/06 16:21:26 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	remove_var(char	var, char **envp)
// {
// 	char	*env_line;
// 	int		i;
// 	int		j;

// 	env_line = getenv(var);
// 	if (!env_line)
// 		return ;
// 	while (envp[i])
// 	{
// 		if (ft_strcmp(env_line, envp[i]) == 0)
// 		{
// 			j = 0;
// 			free(envp[i])
// 			while (envp[i + 1])
// 			{
// 				envp[i] = env
// 			}
// 		}
// 	}

// }

void	free_all(char **arg)
{
	int i;

	i = 0;
	if (!arg)
		return ;
	while (arg[i])
		free(arg[i++]);
	free (arg);
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
		remove_var(variables[i], envp);
	return (free_all(variables), EXIT_SUCCESS);
}
