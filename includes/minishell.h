#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_shell
{
	char	**env;
	int		exit_status;
	char	*line;
}	t_shell;

int	parse_line(t_shell *shell);

#endif