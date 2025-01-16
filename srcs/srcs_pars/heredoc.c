/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuru <kuru@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 20:42:09 by kuru              #+#    #+#             */
/*   Updated: 2025/01/16 21:44:47 by kuru             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void uint_to_hex(unsigned int num, char *hex_str)
{
    const char hex_chars[] = "0123456789abcdef";
    int i;
    
	i = HEX_STRING_LEN - 1;
    while (i >= 0)
    {
        hex_str[i] = hex_chars[num & HEX_MASK];
        num >>= 4;
		i--;
    }
    hex_str[HEX_STRING_LEN] = '\0';
}

static unsigned int	lcg_rand(unsigned int *seed)
{
	return ((*seed * 1664525 + 1013904223) % UINT32_MAX);
}

static char *get_heredoc_filename(void)
{
    static unsigned int seed = 42;
    char *filename;
    char hex_string[HEX_STRING_LEN + 1];

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
	int		fd;
	char	*heredoc_filename;

	heredoc_filename = get_heredoc_filename();
	fd = open(heredoc_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(heredoc_filename);
		return (NULL);
	}
	fill_heredoc(fd, delimiter);
	close(fd);
	return (heredoc_filename);
}