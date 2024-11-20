#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "lexer.h"
# include "ast.h"
# include "colors.h"

// typedef enum e_token
// {
// 	COMMAND,
// 	PIPE,
// 	SINGLE_QUOTE,
// 	DOUBLE_QUOTE,
// 	REDIRECT_IN,
// 	REDIRECT_OUT,
// 	APPEND,
// 	HEREDOC,
// 	FILENAME,
// 	ARGUMENT,
// }	t_token;

// typedef struct s_token
// {
// 	t_token	type;
// 	char	*value;
// }	t_token;

// typedef struct s_env_var {
//     char				*key;
//     char				*value;
//     struct s_env_var	*next;
// }	t_env_var;

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

int	parse_line(t_shell *shell);

#endif