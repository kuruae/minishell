/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:02:26 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/09 21:43:20 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_home(char **envp) // i use this function instead of getenv("HOME") in case the HOME variable has been manipulated by the user
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp("HOME=", envp[i], 5) == 0) // looking for the HOME variable
			return (envp[i] + 5); // shifting it 5 to the right so that the HOME= is cut out
		i++;
	}
	return (NULL);
}

void	skip_space(char	*arg, int *i)
{
	while (ft_isspace(arg[*i]))
		(*i)++;
}

int	arg_count(char	*arg)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	if (!arg)
		return (0);
	if (ft_isspace(arg[i]))
			skip_space(arg, &i);
	while (arg[i])
	{
		counter++;
		while(!ft_isspace(arg[i]) && arg[i])
			i++;
		if (ft_isspace(arg[i]))
			skip_space(arg, &i);
	}
	return (counter);
}

int	ft_cd(char *path, t_directory *dir, char **envp)
{
	char	cache[MAX_PATH];

	ft_printf("starting ft_cd\n");
	if (arg_count(path) > 1)
	{
		return(ft_putstr_fd("total error: cd: too many arguments", 2),
		ft_putchar_fd('\n', 2), EXIT_FAILURE);
	}
	if (getcwd(cache, MAX_PATH) == NULL) //saving old_path variable in the cache
		return (perror("total error: cd"), EXIT_FAILURE);
	if (getcwd(cache, MAX_PATH) == NULL) //saving old_path variable in the cache
		return (perror("total error: cd"), EXIT_FAILURE);
	if (arg_count(path) == 0) // when there is only cd written it redirects to the home directory
	{
		if(!get_home(envp)) // whe use this sub function to look for the HOME= variable in envp
			ft_putstr_fd("total error: cd: no home variable", 2);
		ft_strlcpy(dir->home_path, get_home(envp), MAX_PATH);
		chdir(dir->home_path);
	}
	else if (chdir(path) == -1)
		return (perror("total error: cd"), EXIT_FAILURE);
	ft_strlcpy(dir->old_path, cache, ft_strlen(cache) + 1); // copying cache to old path after directory was succesfully changed
	if (getcwd(dir->current_path, MAX_PATH) == NULL) // setting the new current_path variable
		return (perror("cd error"), (EXIT_FAILURE));
	ft_printf("new directory: %s\nold directory: %s\n", dir->current_path, dir->old_path);
	return (EXIT_SUCCESS);
}
