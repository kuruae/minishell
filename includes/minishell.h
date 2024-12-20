#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "lexing.h"
// # include "ast.h"
# include "colors.h"
# include "get_signal.h"
# include "builtin.h"
# include "exec.h"
# include <sys/wait.h>

// bullshit so i can work on macos
# ifdef __APPLE__
    extern void rl_replace_line(const char*, int);
	extern void rl_clear_history (void);
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
	//values for builtins
}	t_shell;

typedef enum e_error
{
	SUCCESS,
	CTRL_D,
	ERR_FATAL,
	FAILURE
}	t_error;

# include "ast.h"

// gobal functions
int		parse_line(t_shell *shell);

void	clean_up_end(t_shell *shell);
void	clean_up_node(t_ast_node *node);
void	clean_up_parser(t_parser *parser);

#endif
