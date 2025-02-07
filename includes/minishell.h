/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 21:01:47 by enzo              #+#    #+#             */
/*   Updated: 2025/02/07 21:30:17 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "error_types.h"
# include "exec.h"
# include "colors.h"
# include "get_signal.h"
# include "builtin.h"
# include "wildcard.h"
# include "lexing.h"

// macos bullshit i hate readline 
# ifdef __APPLE__
	extern int	rl_done;
    extern void (*rl_event_hook)(void);
    extern void	rl_replace_line(const char*, int);
	extern void	rl_clear_history (void);
	extern int	append_history (int nelements, char *filename);
# endif

# define PROMPT CYAN"petit total"MAGENTA" > "RESET

# define HISTORY_FILE ".shell_history"
# define HISTORY_SIZE 500

extern int	g_sig_offset;

/* 		structs		*/
typedef struct s_env
{
	char			*id;
	char			*value;
	char			*raw;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	char		***envp;
	int			exit_status;
	char		*line;
	t_directory	dir;
	pid_t		pid[MAX_PROCESS_COUNT];
	int			pipes[MAX_PIPE_COUNT][2];
	int			pipe_count;
	int			pipe_index;
	int			process_count;
	int			process_index;
	t_ast_node	*root_node;
	bool		pipeline;
	int			shell_level;
	t_shell		*subshell;
	t_shell		*parent_shell;
}	t_shell;

/*	these headers are lower because of circular dependencies  */
# include "ast.h"


/*		functions		*/
int		parse_line(t_shell *shell);
void	clean_up_end(t_shell *shell);
void	clean_up_node(t_ast_node *node);
void	clean_up_parser(t_parser *parser);
void	free_lexing(t_token *tokens);
void	free_user_input(t_token *tokens, t_ast_node *ast);

void	free_envp(char ***envp);
void	clean_up_child(t_shell *shell);
void	init_shell(t_shell *shell, t_ast_node *node);
#endif
