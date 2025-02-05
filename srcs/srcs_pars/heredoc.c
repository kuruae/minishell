/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 20:42:09 by kuru              #+#    #+#             */
/*   Updated: 2025/02/05 05:20:08 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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

static char	*init_fill_heredoc(char *delimiter)
{
	char	*quoteless_delimiter;
	char	*end_heredoc;

	quoteless_delimiter = remove_quotes_from_string(delimiter);
	end_heredoc = ft_strjoin(quoteless_delimiter, "\n");
	free(quoteless_delimiter);
	ft_putstr_fd("heredoc> ", STDOUT_FILENO);
	return (end_heredoc);
}

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


static void	fill_heredoc(int fd, char *delimiter, char **env, int *sig)
{
	char		*line;
	char		*end_heredoc;

	get_signal_heredoc();
	end_heredoc = init_fill_heredoc(delimiter);
	line = get_next_line(STDIN_FILENO);
	while (line && ft_strcmp(line, end_heredoc))
	{
		if (g_sig_offset == 130)
		{
			*sig = 130;
			free(line);
			break;
		}
		write_current_line(fd, line, env);
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	if (line && !g_sig_offset)
		free(line);
	if (end_heredoc)
		free(end_heredoc);
}

char	*heredoc_handler(char *delimiter, char **env)
{
	int		fd;
	int		sig;
	char	*heredoc_filename;

	sig = 0;
	heredoc_filename = get_heredoc_filename();
	fd = open(heredoc_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(heredoc_filename);
		return (NULL);
	}
	fill_heredoc(fd, delimiter, env, &sig);
	if (sig == 130)
	{
		unlink(heredoc_filename);
		free(heredoc_filename);
		close(fd);
		return (NULL);
	}
	close(fd);
	return (heredoc_filename);
}