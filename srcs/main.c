/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:59:17 by enzo              #+#    #+#             */
/*   Updated: 2024/11/20 14:18:28 by emagnani         ###   ########.fr       */
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
	// t_env env;

	(void)argc;
	(void)argv;
	// env = init_env(envp); // envp is an array of strings btw (and i didnt know the p stands for pointer)
	shell.exit_status = 0;
	shell.envp = envp;
	shell.line = NULL;
	shell.line = readline(cyan"petit total"magenta" > "reset);
	while (shell.line)
	{
		if (shell.line[0] != '\0')
		{
			// add_history(shell.line);
			parse_line(&shell);
		}
		free(shell.line);
		shell.line = readline(cyan"petit total"magenta" > "reset);
	}
	return (0);
}