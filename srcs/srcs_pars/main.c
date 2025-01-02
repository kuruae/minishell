/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:59:17 by enzo              #+#    #+#             */
/*   Updated: 2024/12/25 10:46:17 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_sig_offset = 0;

// int	parse_line(t_shell *shell)
// {
// 	char	**args;
// 	int		i;

// 	args = ft_split(shell->line, ' ');
// 	i = 0;
// 	while (args[i])
// 	{
// 		printf("args[%d] = %s\n", i, args[i]);
// 		i++;
// 	}
// 	return (0);
// }

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
		case TOK_EXPAND: return "EXPAND";
		case TOK_WILDCARD: return "WILDCARD";
        default: return "UNKNOWN";
    }
}

int test_lexing(char *line)
{
    t_token *tokens;
    t_token *current;
    int     i;

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

t_error readline_loop(t_shell *shell)
{
	t_exec_error	status;

	init_history();
	shell->line = readline(PROMPT);
	while (shell->line)
	{
		if (!is_line_empty(shell->line))
		{
			// ast(lexing(shell->line));
			// add_history(shell->line);
			// parse_line(shell);
			//printf("\nLexing:\n");
			//test_lexing(shell->line);
			t_token *tokens = lexing(shell->line);
			t_ast_node *ast = parse_tokens(tokens);
			printf("\nAST Structure:\n");
    		debug_print_ast(ast, 0);
			add_history(shell->line);
			append_history(1, HISTORY_FILE);
			status = start_exec(shell, ast);
			if (status == EXEC_ERR_FATAL)
				return (ERR_FATAL);
			free_user_input(tokens, ast);
		}
		free(shell->line);
		shell->line = readline(PROMPT);
	}
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
	get_signal();
	status = readline_loop(&shell);
	if (status == ERR_FATAL)
		return(clean_up_end(&shell), 1);
	if (status == CTRL_D)
		g_sig_offset = 0;
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
