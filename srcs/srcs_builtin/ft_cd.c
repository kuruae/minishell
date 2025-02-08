/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:02:26 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/07 21:05:23 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exec_error	update_oldpwd(char ***envp, t_directory *dir)
{
	char			*arg;
	char			**args;
	t_exec_error	status;

	(void)envp;
	arg = ft_strjoin("OLDPWD=", dir->old_path);
	if (!arg)
		return (EXEC_ERR_FATAL);
	args = malloc(sizeof(char *) * 2);
	if (!args)
		return (free(arg), EXEC_ERR_FATAL);
	args[0] = arg;
	args[1] = NULL;
	status = ft_export(args, 1, envp, 1);
	free(arg);
	free(args);
	return (status);
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
	status = ft_export(args, 1, envp, 1);
	free(arg);
	free(args);
	status = update_oldpwd(envp, dir);
	return (status);
}

t_exec_error	update_t_directory(t_directory *dir, char *cache, char ***envp)
{
	ft_strlcpy(dir->old_path, cache, ft_strlen(cache) + 1);
	if (getcwd(dir->current_path, MAX_PATH) == NULL)
		return (perror("cd error"), EXEC_ERR_NON_FATAL);
	return (update_pwd(envp, dir));
}

t_exec_error	get_home(char **envp, t_directory *dir, char *cache)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp("HOME=", envp[i], 5) == 0)
		{
			ft_strlcpy(dir->home_path, envp[i] + 5, MAX_PATH);
			chdir(dir->home_path);
			return (update_t_directory(dir, cache, &envp));
		}
		i++;
	}
	ft_putstr_fd("total error: cd: no home variable", 2);
	return (EXEC_ERR_NON_FATAL);
}

t_exec_error	ft_cd(char **args, int argc, t_directory *dir, t_shell *shell)
{
	char	cache[MAX_PATH];

	if (argc > 1)
	{
		return (ft_putstr_fd("total error: cd: too many argument\n", 2),
			EXEC_ERR_NON_FATAL);
	}
	if (getcwd(cache, MAX_PATH) == NULL)
		ft_strlcpy(cache, dir->current_path, MAX_PATH);
	if (argc == 0)
		return (get_home(*shell->envp, dir, cache));
	else if (chdir(args[0]) == -1)
	{
		if (ft_strcmp(args[0], "..") == 0)
		{
			if (chdir(dir->old_path) == -1)
				return (perror("total error: cd"), EXEC_ERR_NON_FATAL);
		}
		return (perror("total error: cd"), EXEC_ERR_NON_FATAL);
	}
	return (update_t_directory(dir, cache, shell->envp));
}
