/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:02:26 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/20 02:23:32 by jbaumfal         ###   ########.fr       */
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

t_exec_error	ft_cd(char **args, int argc, t_directory *dir, char **envp)
{
	char	cache[MAX_PATH];

	if (argc > 1)
	{
		return(ft_putstr_fd("total error: cd: too many arguments", 2),
		ft_putchar_fd('\n', 2), EXEC_ERR_NON_FATAL);
	}
	if (getcwd(cache, MAX_PATH) == NULL) //saving old_path variable in the cache
		return (perror("total error: cd"), EXEC_ERR_NON_FATAL);
	if (getcwd(cache, MAX_PATH) == NULL) //saving old_path variable in the cache
		return (perror("total error: cd"), EXEC_ERR_NON_FATAL);
	if (argc == 0) // when there is only cd written it redirects to the home directory
	{
		if(!get_home(envp)) // whe use this sub function to look for the HOME= variable in envp
			ft_putstr_fd("total error: cd: no home variable", 2);
		ft_strlcpy(dir->home_path, get_home(envp), MAX_PATH);
		chdir(dir->home_path);
	}
	else if (chdir(args[0]) == -1)
		return (perror("total error: cd"), EXEC_ERR_NON_FATAL);
	ft_strlcpy(dir->old_path, cache, ft_strlen(cache) + 1); // copying cache to old path after directory was succesfully changed
	if (getcwd(dir->current_path, MAX_PATH) == NULL) // setting the new current_path variable
		return (perror("cd error"), EXEC_ERR_NON_FATAL);
	//ft_printf("new directory: %s\nold directory: %s\n", dir->current_path, dir->old_path);
	return (EXEC_SUCCESS);
}
