/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:18:10 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/21 00:33:33 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_env(char ***envp)
{
	int	i;

	i = 0;
	if (!*envp)
		return ;
	while ((*envp)[i])
	{
		free((*envp)[i]);
		i++;
	}
	free(*envp);
	free(envp);
}

void	clean_up_end(t_shell *shell)
{
	rl_clear_history();
	free_env(shell->envp);
}
