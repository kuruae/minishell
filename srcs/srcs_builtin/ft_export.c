/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:45:22 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/13 22:06:22 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_var(char *var)
{
	int	i;

	i = 0;
	while (var[i])
	{
		if (var[i] == '=')
			break ;
		i++;
	}
	if (i == 0) // case when the variab;e starts with an '='
		return (-1); // in this case an error is returned like in the real bash
	if (!var[i]) // case when there is no '=' in the string
		return (0); // in this case the function is suposed to do nothing -> no error
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

int	replace_var(char *var,char ***envp)
{
	char **line;

	line = find_var(var, *envp);
	free(*line);
	*line = ft_strdup(var);
	if (line == NULL)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}



int	add_var(char *var, char ***envp)
{
	int	i;
	char **new_envp; // as the envp will become one yard longer we have to create a new array and malloc one line more

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

t_bi_error	ft_export(char *arg,char ***envp)
{
	char	**variables;
	int		i;

	i = 0;
	if (!arg)
		return (BI_SUCCESS);
	variables = ft_split(arg, ' ');
	if (!variables)
		return (free_all(variables), BI_ERR_MALLOC);
	while (variables[i])
	{
		if (check_var(variables[i]) == -1) // var starts with = -> error message
			ft_putstr_fd("export: no valid identifier\n", 2); // in real bash the variable is mentioned (for this we would need a printf that prints on std_error)
		else if (check_var(variables[i]) == 0) // var has no = -> do nothinf
				;
		else if (find_var(variables[i], *envp) != NULL)
			replace_var(variables[i], envp);
		else
			add_var(variables[i], envp);
		i++;
	}
	ft_env(*envp, 1);
	return (free_all(variables), BI_SUCCESS);
}
