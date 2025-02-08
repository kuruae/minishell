/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:59:17 by enzo              #+#    #+#             */
/*   Updated: 2025/02/08 02:22:22 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int					g_sig_offset = 0;
static const char	*g_prompt = CYAN "petit total" MAGENTA " > " RESET;

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

static t_error	user_intput_routine(t_shell *shell)
{
	t_exec_error	status;
	t_token			*tokens;
	t_ast_node		*ast;

	tokens = lexing(shell->line);
	add_history(shell->line);
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

	shell->line = readline(g_prompt);
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
		shell->line = readline(g_prompt);
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
	rl_clear_history();
	if (status == ERR_FATAL)
		return (clean_up_end(&shell), EXIT_FAILURE);
	if (status == CTRL_D)
		g_sig_offset = 131;
	clean_up_end(&shell);
	return (0);
}
