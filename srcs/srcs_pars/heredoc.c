/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 20:42:09 by kuru              #+#    #+#             */
/*   Updated: 2025/02/07 19:41:10 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const t_sig *const	g_sig = &(t_sig){.sig = 0};

static void	check_interrupt(void)
{
	if (((t_sig *)g_sig)->sig == SIGINT)
		rl_done = 1;
}

static void	sigint_handler(int sig)
{
	(void)sig;
	((t_sig *)g_sig)->sig = SIGINT;
}

static void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static int	read_loop(int fd, char *delim, char **env)
{
	char	*line;

	rl_event_hook = check_interrupt;
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
			ft_putstr_fd("warning: here-doc delimited by EOF\n", 2);
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

	((t_sig *)g_sig)->sig = 0;
	filename = get_heredoc_filename();
	if (!filename)
		return (NULL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (free(filename), NULL);
	setup_heredoc_signals();
	status = read_loop(fd, delimiter, env);
	close(fd);
	rl_event_hook = NULL;
	if (status == 130)
		return (unlink(filename), free(filename), NULL);
	return (filename);
}
