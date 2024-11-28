#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "lexing.h"
# include "ast.h"
# include "colors.h"

# define PROMPT cyan"petit total"magenta" > "reset
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

int	parse_line(t_shell *shell);

#endif