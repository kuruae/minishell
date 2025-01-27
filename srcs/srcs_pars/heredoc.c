/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 20:42:09 by kuru              #+#    #+#             */
/*   Updated: 2025/01/27 02:30:40 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Converts an unsigned integer to its hexadecimal string representation
 * 
 * @param num      The unsigned integer to convert
 * @param hex_str  Output buffer for the hex string
 * 
 * Converts each 4-bit chunk of the input number into a hex character (0-f)
 * and stores it in the provided buffer. Adds null terminator at the end.
 */
static void	uint_to_hex(unsigned int num, char *hex_str)
{
	const char	hex_chars[] = "0123456789abcdef";
	int			i;

	i = HEX_STRING_LEN - 1;
	while (i >= 0)
	{
		hex_str[i] = hex_chars[num & HEX_MASK];
		num >>= 4;
		i--;
	}
	hex_str[HEX_STRING_LEN] = '\0';
}

/**
 * Linear Congruential Generator (LCG) for pseudo-random number generation
 * 
 * @param seed  Pointer to the seed value that will be updated
 * @return      The next pseudo-random number in the sequence
 * 
 * Uses the formula: next = (seed * 1664525 + 1013904223) % UINT32_MAX
 * Parameters chosen for good statistical properties
 * We use this to generate unique filenames for heredoc temporary files
 * in case we need to create multiple heredocs in the same session.
 */
static unsigned int	lcg_rand(unsigned int *seed)
{
	return ((*seed * 1664525 + 1013904223) % UINT32_MAX);
}

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

static void	fill_heredoc(int fd, char *delimiter)
{
	char	*line;
	char	*end_heredoc;

	end_heredoc = ft_strjoin(delimiter, "\n");
	ft_putstr_fd("heredoc> ", STDOUT_FILENO);
	line = get_next_line(STDIN_FILENO);
	while (line && ft_strcmp(line, end_heredoc))
	{
		write(fd, line, ft_strlen(line));
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	free(end_heredoc);
}

char	*heredoc_handler(char *delimiter)
{
	pid_t 	pid;
	int		fd;
	char	*heredoc_filename;
	int		status;

	heredoc_filename = get_heredoc_filename();
	fd = open(heredoc_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(heredoc_filename);
		return (NULL);
	}
	pid	= fork();
	if (pid < 0)
		return (NULL);
	if (pid == 0)
	{
		get_signal_heredoc();
		fill_heredoc(fd, delimiter);
		close(fd);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		close(fd);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_sig_offset = WEXITSTATUS(status);
	}
	return (heredoc_filename);
}
