/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:45:22 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/05 17:10:45 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_var(char *var)
{
	int	i;

	i = 0;
	while (var[i])
	{
		if (var[i] == '=')
			break ;
		i++;
	}
	if (i == 0)
		return (-1);
	if (!var[i])
		return (0);
	return (i);
}

char	**find_var(char	*var, char **envp)
{
	int		i;
	size_t	len;

	i = 0;
	len = check_var(var);
	while (envp[i])
	{
		if (ft_strncmp(var, envp[i], len) == 0)
			return (&envp[i]);
		i++;
	}
	return (NULL);
}

int	replace_var(char *var, char ***envp)
{
	char	**line;

	line = find_var(var, *envp);
	free(*line);
	*line = ft_strdup(var);
	if (line == NULL)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	add_var(char *var, char ***envp)
{
	int		i;
	char	**new_envp;

	i = 0;
	while ((*envp)[i])
		i++;
	new_envp = (char **)malloc((i + 2) * sizeof(char *));
	i = 0;
	while ((*envp)[i])
	{
		new_envp[i] = ft_strdup((*envp)[i]);
		i++;
	}
	new_envp[i] = ft_strdup(var);
	new_envp[i + 1] = NULL;
	free_all(*envp);
	*envp = new_envp;
	return (EXIT_SUCCESS);
}

t_exec_error	ft_export(char **args, int argc, char ***envp)
{
	int		i;

	i = 0;
	if (argc == 0)
		return (EXEC_SUCCESS);
	while (args[i])
	{
		if (check_var(args[i]) == -1)
			ft_putstr_fd("export: no valid identifier\n", 2);
		else if (check_var(args[i]) == 0)
			;
		else if (find_var(args[i], *envp) != NULL)
			replace_var(args[i], envp);
		else
			add_var(args[i], envp);
		i++;
	}
	return (EXEC_SUCCESS);
}
