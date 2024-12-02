#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "lexing.h"
# include "ast.h"
# include "colors.h"
# include "get_signal.h"

// define readline prompt
# define PROMPT cyan"petit total"magenta" > "reset

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
	char	**envp;
	int		exit_status;
	char	*line;
}	t_shell;

typedef enum e_error
{
	SUCCESS,
	ERR_MALLOC,
	FAILURE
}	t_error;

// functions
int		parse_line(t_shell *shell);
void	clean_up(t_shell *shell);

#endif
