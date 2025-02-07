/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 20:42:09 by kuru              #+#    #+#             */
/*   Updated: 2025/02/07 05:38:35 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const t_sig *const	g_sig = &(t_sig){.sig = 0};
/**
 * Generates a unique filename for heredoc temporary files
 * 
 * @return  Dynamically alloc string containing the filename, or NULL on error
 * 
 * Creates a filename by combining HEREDOC_PREFIX with a random hex string.
 * If the generated filename already exists, recursively tries again.
 */
char	*get_heredoc_filename(void)
{
	static unsigned int	seed = 42;
	char				*filename;
	char				hex_string[HEX_STRING_LEN + 1];

	seed = lcg_rand(&seed);
	uint_to_hex(seed, hex_string);
	filename = ft_strjoin(HEREDOC_PREFIX, hex_string);
	if (!filename)
		return (NULL);
	if (access(filename, F_OK) == 0)
	{
		free(filename);
		return (get_heredoc_filename());
	}
	return (filename);
}

bool	is_expansion_on(char *delemiter)
{
	if (!delemiter)
		return (true);
	if (delemiter[0] == '\'' && delemiter[ft_strlen(delemiter) - 1] == '\'')
		return (false);
	return (true);
}

// static char	*init_fill_heredoc(char *delimiter)
// {
// 	char	*quoteless_delimiter;
// 	char	*end_heredoc;

// 	quoteless_delimiter = remove_quotes_from_string(delimiter);
// 	end_heredoc = ft_strjoin(quoteless_delimiter, "\n");
// 	free(quoteless_delimiter);
// 	ft_putstr_fd("heredoc> ", STDOUT_FILENO);
// 	return (end_heredoc);
// }

static void	write_current_line(int fd, char *line, char **env)
{
	char	*expanded_line;

	if (is_expansion_on(NULL))
	{
		expanded_line = expand_env_vars(line, env);
		write(fd, expanded_line, ft_strlen(expanded_line));
		free(expanded_line);
	}
	else
		write(fd, line, ft_strlen(line));
}

static void	sigint_handler(int sig)
{
	(void)sig;
	((t_sig *)g_sig)->sig = SIGINT;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

static void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static bool	check_delim_match(char *input, char *delimiter)
{
	char	*clean_delim;
	bool	match;

	clean_delim = remove_quotes_from_string(delimiter);
	match = (ft_strcmp(input, clean_delim) == 0);
	free(clean_delim);
	return (match);
}

static int	read_loop(int fd, char *delim, char **env)
{
	char	*line;

	while (true)
	{
		line = readline("heredoc> ");
		if (((t_sig *)g_sig)->sig == SIGINT)
		{
			free(line);
			return (130);
		}
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-doc delimited by EOF\n", 2);
			return (0);
		}
		if (check_delim_match(line, delim))
		{
			free(line);
			break ;
		}
		write_current_line(fd, line, env);
		free(line);
	}
	return (0);
}

char	*heredoc_handler(char *delimiter, char **env)
{
	int		fd;
	int		status;
	char	*filename;

	filename = get_heredoc_filename();
	if (!filename)
		return (NULL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (free(filename), NULL);
	setup_heredoc_signals();
	status = read_loop(fd, delimiter, env);
	close(fd);
	if (status == 130)
		return (unlink(filename), free(filename), NULL);
	return (filename);
}