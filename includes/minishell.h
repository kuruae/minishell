#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "exec.h"
// # include "ast.h"
# include "colors.h"
# include "get_signal.h"
# include "builtin.h"
# include <sys/wait.h>

// bullshit so i can work on macos
# ifdef __APPLE__
    extern void	rl_replace_line(const char*, int);
	extern void	rl_clear_history (void);
	extern int	append_history (int nelements, char *filename);
# endif

// define readline prompt
# define PROMPT cyan"petit total"magenta" > "reset

# define HISTORY_FILE ".shell_history"
# define HISTORY_SIZE 500

// global variable
extern int	g_sig_offset;

// structs
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
	int			process_count; //amount of child processes
	int			process_index;
	t_ast_node	*root_node;
	bool		pipeline;
}	t_shell;

typedef enum e_error
{
	SUCCESS,
	CTRL_D,
	ERR_FATAL,
	ERR_MALLOC,
	ERR_SYNTAX,
	FAILURE,
	CTRL_C = 130
}	t_error;

# include "lexing.h"
# include "ast.h"
# include "wildcard.h"

// general functions
int		parse_line(t_shell *shell);
void	clean_up_end(t_shell *shell);
void	clean_up_node(t_ast_node *node);
void	clean_up_parser(t_parser *parser);
void	free_lexing(t_token *tokens);
void	free_user_input(t_token *tokens, t_ast_node *ast);

#endif
