/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:02:26 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/01/26 17:32:24 by jbaumfal         ###   ########.fr       */
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

t_exec_error	update_pwd(char ***envp, t_directory *dir)
{
	char			*arg;
	char			**args;
	t_exec_error	status;

	(void)envp;
	arg = ft_strjoin("PWD=", dir->current_path);
	if (!arg)
		return (EXEC_ERR_FATAL);
	args = malloc(sizeof(char *) * 2);
	if (!args)
		return (EXEC_ERR_FATAL);
	args[0] = arg;
	args[1] = NULL;
	status = ft_export(args, 1, envp);
	free(arg);
	free(args);
	return (status);
}

t_exec_error	ft_cd(char **args, int argc, t_directory *dir, char ***envp)
{
	char	cache[MAX_PATH];

	
	if (argc > 1)
	{
		return(ft_putstr_fd("total error: cd: too many arguments", 2),
		ft_putchar_fd('\n', 2), EXEC_ERR_NON_FATAL);
	}
	if (getcwd(cache, MAX_PATH) == NULL) //saving old_path variable in the cache
		ft_strlcpy(cache, dir->current_path, MAX_PATH);
	if (argc == 0) // when there is only cd written it redirects to the home directory
	{
		if(!get_home(*envp)) // whe use this sub function to look for the HOME= variable in envp
			ft_putstr_fd("total error: cd: no home variable", 2);
		ft_strlcpy(dir->home_path, get_home(*envp), MAX_PATH);
		chdir(dir->home_path);
	}
	else if (chdir(args[0]) == -1)
	{
		if (ft_strcmp(args[0], "..") == 0)
		{
			if (chdir(dir->old_path) == -1)
				return (perror("total error: cd"), EXEC_ERR_NON_FATAL);
		}
		return (perror("total error: cd"), EXEC_ERR_NON_FATAL);
	}
	ft_strlcpy(dir->old_path, cache, ft_strlen(cache) + 1); // copying cache to old path after directory was succesfully changed
	if (getcwd(dir->current_path, MAX_PATH) == NULL) // setting the new current_path variable
		return (perror("cd error"), EXEC_ERR_NON_FATAL);
	return (update_pwd(envp, dir));
}
