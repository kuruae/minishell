/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:59:17 by enzo              #+#    #+#             */
/*   Updated: 2024/12/02 14:24:44 by jbaumfal         ###   ########.fr       */
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

int	readline_loop(t_shell *shell)
{
	shell->line = readline(PROMPT);
	while (shell->line)
	{
		if (shell->line[0] != '\0')
		{
			// ast(lexing(shell->line));
			// add_history(shell->line);
			// parse_line(shell);
			test_lexing(shell->line);
		}
		free(shell->line);
		shell->line = readline(PROMPT);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	// t_env env;
	// int	g_sig_offset;

	(void)argc;
	(void)argv;
	// env = init_env(envp); // envp is an array of strings btw (and i didnt know the p stands for pointer)
	shell.exit_status = 0;
	shell.envp = envp;
	shell.line = NULL;
	get_signal();
	if (readline_loop(&shell) == 0)
		g_sig_offset = 0;
	clean_up(&shell);
	return (0);
}
