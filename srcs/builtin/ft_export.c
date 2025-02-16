/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@42.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:45:22 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/16 04:56:31 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_var(char *var)
{
	int	i;

	i = 0;
	if (ft_is_num(var) == 1 || ft_isdigit(var[0]) == 1)
	{
		g_sig_offset = 1;
		return (-1);
	}
	while (var[i] && var[i] != '=')
	{
		if (ft_isalnum(var[i]) == 0 && var[i] != '_')
			return (-1);
		i++;
	}
	i = 0;
	while (var[i])
	{
		if (var[i] == '=')
			break ;
		i++;
	}
	if (i == 0)
		return (-1);
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


/*
	This function is used to imitate the real export call in bash.
	- First we check the number of arguments given (if there is no argument we print the whole environment).
	- Then we check argument by argument if it is a valid identifier.
		- If it is not we print an error message.
		- If the argument is a valid identifier we check if it is already in the environment.
			- If it is we replace the variable.
			- If it is not we add the variable to the environment.
*/

t_exec_error	ft_export(char **args, int argc, char ***envp, int fd)
{
	t_exec_error	status;
	int				i;

	status = EXEC_SUCCESS;
	i = 0;
	if (argc == 0 || ft_strlen(args[0]) == 0)
		return (print_export_env(*envp, fd));
	while (args[i])
	{
		if (check_var(args[i]) == -1)
		{
			print_error("export", args[i], "not a valid identifier");
			status = EXEC_ERR_NON_FATAL;
		}
		else if (check_var(args[i]) == 0)
			;
		else if (find_var(args[i], *envp) != NULL)
			replace_var(args[i], envp);
		else
			add_var(args[i], envp);
		i++;
	}
	return (status);
}
