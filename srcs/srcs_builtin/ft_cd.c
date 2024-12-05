/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:02:26 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/05 16:02:21 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(char *path, t_directory *dir)
{
	char	cache[MAX_PATH];
	// char	absolute_path[MAX_PATH];

	if (path[0])
	if (getcwd(cache, MAX_PATH) == NULL) //saving old_path variable in the cache
		return (perror("cd error"), EXIT_FAILURE);
	chdir(path);
	// if (chdir(path) != EXIT_SUCCESS)
	// 	return (perror("cd error"), (EXIT_FAILURE));
	ft_strlcpy(dir->old_path, cache, ft_strlen(cache)); // copying cache to old path after directory was succesfully changed
	if (getcwd(dir->current_path, MAX_PATH) == NULL) // setting the new current_path variable
		return (perror("cd error"), (EXIT_FAILURE));
	ft_printf("new directory: %s\nold directory: %s\n", dir->current_path, dir->old_path);
	return (EXIT_SUCCESS);
}

// int	main(int argc, char **argv)
// {
// 	if (argc == 2)
// 		return (ft_cd(argv[1]));
// }
