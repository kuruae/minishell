/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:59:17 by enzo              #+#    #+#             */
/*   Updated: 2025/02/07 20:17:55 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_sig_offset = 0;

static bool	is_line_empty(char *line)
{
	while (*line)
	{
		if (!ft_isspace(*line))
			return (false);
		line++;
	}
	return (true);
}

static void	init_history(void)
{
	int	file;

	file = open(HISTORY_FILE, O_CREAT | O_RDWR, 0644);
	stifle_history(HISTORY_SIZE);
	history_truncate_file(HISTORY_FILE, HISTORY_SIZE);
	read_history(HISTORY_FILE);
	close(file);
}

static t_error	user_intput_routine(t_shell *shell)
{
	t_exec_error	status;
	t_token			*tokens;
	t_ast_node		*ast;

	tokens = lexing(shell->line);
	add_history(shell->line);
	append_history(1, HISTORY_FILE);
	if (!tokens)
	{
		g_sig_offset = 2;
		return (ERR_SYNTAX);
	}
	ast = ast_handler(tokens, shell->envp);
	free_lexing(tokens);
	if (!ast)
		return (free_user_input(tokens, ast), CTRL_C);
	status = start_exec(shell, ast);
	if (status == EXEC_ERR_FATAL)
		return (ERR_FATAL);
	free_user_input(tokens, ast);
	return (SUCCESS);
}

t_error	readline_loop(t_shell *shell)
{
	t_error	routine_status;

	init_history();
	shell->line = readline(PROMPT);
	while (shell->line)
	{
		if (!shell->line)
			break ;
		if (!is_line_empty(shell->line))
		{
			routine_status = user_intput_routine(shell);
			if (routine_status == ERR_FATAL)
				return (ERR_FATAL);
			if (routine_status == ERR_SYNTAX)
				ft_putstr_fd("minishell: syntax error\n", STDERR_FILENO);
		}
		free(shell->line);
		get_signal_interactive();
		shell->line = readline(PROMPT);
	}
	g_sig_offset = 131;
	ft_printf("exit\n");
	return (CTRL_D);
}

char	***copy_env(char **envp)
{
	int		i;
	char	***env_cpy;

	i = 0;
	while (envp[i])
		i++;
	env_cpy = malloc(sizeof(char **));
	(*env_cpy) = (char **)malloc((i + 1) * sizeof(char *));
	i = 0;
	while (envp[i])
	{
		(*env_cpy)[i] = ft_strdup(envp[i]);
		i++;
	}
	(*env_cpy)[i] = NULL;
	return (env_cpy);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	t_error	status;

	(void)argc;
	(void)argv;
	shell.exit_status = 0;
	shell.envp = copy_env(envp);
	shell.line = NULL;
	get_signal_interactive();
	status = readline_loop(&shell);
	if (status == ERR_FATAL)
		return (clean_up_end(&shell), EXIT_FAILURE);
	if (status == CTRL_D)
		g_sig_offset = 131;
	clean_up_end(&shell);
	return (0);
}
