/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_rand_name.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 19:25:35 by enzo              #+#    #+#             */
/*   Updated: 2025/02/07 19:28:03 by enzo             ###   ########.fr       */
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
void	uint_to_hex(unsigned int num, char *hex_str)
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
unsigned int	lcg_rand(unsigned int *seed)
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

static void	write_current_line(int fd, char *line, char **env)
{
	char	*expanded_line;

	if (is_expansion_on(NULL))
	{
		expanded_line = expand_env_vars(line, env);
		write(fd, expanded_line, ft_strlen(expanded_line));
		write(fd, "\n", 1);
		free(expanded_line);
	}
	else
		write(fd, line, ft_strlen(line));
}
