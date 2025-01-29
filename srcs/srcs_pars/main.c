/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:59:17 by enzo              #+#    #+#             */
/*   Updated: 2025/01/29 15:37:41 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_sig_offset = 0;

static const char *get_token_type_str(t_token_type type)
{
    switch (type)
    {
        case TOK_WORD: return "COMMAND";
        case TOK_PIPE: return "PIPE";
        case TOK_REDIR_IN: return "REDIR_IN";
        case TOK_REDIR_OUT: return "REDIR_OUT";
        case TOK_HEREDOC: return "HEREDOC";
        case TOK_APPEND: return "APPEND";
        case TOK_OR: return "OR";
        case TOK_AND: return "AND";
		case TOK_PAR_OPEN: return "OPEN PARENTHESE";
		case TOK_PAR_CLOSE: return "CLOSE PARENTHESE";
        default: return "UNKNOWN";
    }
}

int test_lexing(char *line)
{
    t_token *tokens;
    t_token *current;
    int     i;

	printf("Lexing:\n");
    tokens = lexing(line);
    if (!tokens)
        return (1);

    current = tokens;
    i = 0;
    while (current)
    {
        printf("token[%d] = { type: %s, value: '%s' }\n",
               i,
               get_token_type_str(current->type),
               current->value);
		if (current->expands)
			printf("expands: true\n");
        current = current->next;
        i++;
    }

    free_tokens(tokens);
    return (0);
}

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

	test_lexing(shell->line); // debug function
	tokens = lexing(shell->line);
	if (!tokens)
		return (ERR_SYNTAX);
	ast = ast_handler(tokens, shell->envp);
	debug_print_ast(ast, 0); // debug function
	add_history(shell->line);
	append_history(1, HISTORY_FILE);
	status = start_exec(shell, ast);
	if (status == EXEC_ERR_FATAL)
		return (ERR_FATAL);
	free_user_input(tokens, ast);
	return (SUCCESS);
}

t_error readline_loop(t_shell *shell)
{
	t_error	routine_status;
	
	init_history();
	shell->line = readline(PROMPT);
	while (shell->line)
	{
		if (!shell->line)
            break;
		if (!is_line_empty(shell->line))
		{
			routine_status = user_intput_routine(shell);
			if (routine_status == ERR_FATAL)
				return (ERR_FATAL);
			if (routine_status == ERR_SYNTAX)
				ft_putstr_fd("minishell: syntax error\n", STDERR_FILENO);
		}
		free(shell->line);
		if (g_sig_offset == 130)
			ft_printf("\n");
		get_signal_interactive();
		shell->line = readline(PROMPT);
	}
	g_sig_offset = 131;
	return (CTRL_D);
}
char ***copy_env(char **envp)
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
	// t_env env;
	(void)argc;
	(void)argv;
	// env = init_env(envp); // envp is an array of strings btw (and i didnt know the p stands for pointer)
	shell.exit_status = 0;
	shell.envp = copy_env(envp);
	shell.line = NULL; // maybe its better to copy the envp in a new table
	get_signal_interactive();
	status = readline_loop(&shell);
	if (status == ERR_FATAL)
		return(clean_up_end(&shell), EXIT_FAILURE);
	if (status == CTRL_D)
		g_sig_offset = 131;
	clean_up_end(&shell);
	return (0);
}
/*main to test builtins*/

/*
int	main(int argc, char **argv, char **envp)
{
	char ***envp_cpy;

	envp_cpy = copy_env(envp);
	if (argc == 3)
		builtin(argv[1], argv[2], STDOUT_FILENO, envp_cpy);
	else if (argc == 2)
		builtin(argv[1], NULL , STDOUT_FILENO, envp_cpy);
	free_all(*envp_cpy);
	free(envp_cpy);
}
*/
