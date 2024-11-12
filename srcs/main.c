/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:59:17 by enzo              #+#    #+#             */
/*   Updated: 2024/11/12 02:02:14 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_line(t_shell *shell)
{
	char	**args;
	int		i;

	args = ft_split(shell->line, ' ');
	i = 0;
	while (args[i])
	{
		printf("args[%d] = %s\n", i, args[i]);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.env = envp;
	shell.exit_status = 0;
	shell.line = NULL;
	shell.line = readline("minishell$ ");
	while (shell.line)
	{
		if (shell.line[0] != '\0')
		{
			// add_history(shell.line);
			parse_line(&shell);
		}
		free(shell.line);
		shell.line = readline("minishell$ ");
	}
	return (0);
}