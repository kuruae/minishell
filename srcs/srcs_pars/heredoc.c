/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 20:42:09 by kuru              #+#    #+#             */
/*   Updated: 2025/01/23 17:35:44 by enzo             ###   ########.fr       */
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
static char	*get_heredoc_filename(void)
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

static bool	is_expansion_on(char *delemiter)
{
	if (!delemiter)
		return (true);
	if (delemiter[0] == '\'' && delemiter[ft_strlen(delemiter) - 1] == '\'')
		return (false);
	return (true);
}

static void	fill_heredoc(int fd, char *delimiter, char **env)
{
	char	*line;
	char	*end_heredoc;
	char	*quoteless_delimiter;

	quoteless_delimiter = remove_quotes_from_string(delimiter);
	end_heredoc = ft_strjoin(quoteless_delimiter, "\n");
	free(quoteless_delimiter);
	ft_putstr_fd("heredoc> ", STDOUT_FILENO);
	line = get_next_line(STDIN_FILENO);
	while (line && ft_strcmp(line, end_heredoc))
	{
		if (is_expansion_on(delimiter))
			line = expand_env_vars(line, env);
		write(fd, line, ft_strlen(line));
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	free(end_heredoc);
}

char	*heredoc_handler(char *delimiter, char **env)
{
	int		fd;
	char	*heredoc_filename;

	heredoc_filename = get_heredoc_filename();
	fd = open(heredoc_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(heredoc_filename);
		return (NULL);
	}
	fill_heredoc(fd, delimiter, env);
	close(fd);
	return (heredoc_filename);
}
