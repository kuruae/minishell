/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:09:16 by enzo              #+#    #+#             */
/*   Updated: 2025/01/16 18:32:25 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*create_ast_node(t_node_type type)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->redirections = NULL;
	ft_memset(&node->data, 0, sizeof(node->data));
	return (node);
}

bool	_parser_is_token_type_redir(t_token_type type)
{
	return (type == TOK_REDIR_IN || type == TOK_REDIR_OUT
		|| type == TOK_APPEND || type == TOK_HEREDOC);
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

static char	*get_heredoc_filename(void)
{
	char		*filename;
	char		*char_count;
	static int	int_count = 0;

	char_count = ft_itoa(int_count);
	if (!char_count)
		return (NULL);
	int_count++;
	filename = ft_strjoin("/tmp/heredoc_", char_count);
	free(char_count);
	if (!filename)
		return (NULL);
	if (access(filename, F_OK) == 0)
	{
		free(filename);
		return (get_heredoc_filename());
	}
	return (filename);
}


static char	*heredoc_handler(char *delemiter)
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
	fill_heredoc(fd, delemiter);
	close(fd);
	return (heredoc_filename);
}


t_redir	*create_redir_node(t_token *token, char *file)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	if (token->type == TOK_REDIR_IN)
		redir->type = REDIR_INPUT;
	else if (token->type == TOK_REDIR_OUT)
		redir->type = REDIR_OUTPUT;
	else if (token->type == TOK_APPEND)
		redir->type = REDIR_APPEND;
	else if (token->type == TOK_HEREDOC)
	{
		redir->type = REDIR_HEREDOC;
		redir->file = heredoc_handler(file);
	}
	else
	{
		free(redir);
		return (NULL);
	}
	if (redir->type != REDIR_HEREDOC)
		redir->file = ft_strdup(file);
	redir->next = NULL;
	return (redir);
}

int	count_args(t_token *token)
{
	int	count;

	count = 0;
	while (token && token->type == TOK_WORD)
	{
		count++;
		token = token->next;
	}
	return (count);
}

void	paser_advance(t_parser *parser)
{
	if (parser->current->type != TOK_EOF)
		parser->current = parser->current->next;
}

t_node_type	get_node_type(t_token_type token_type)
{
	if (token_type == TOK_AND)
		return (NODE_AND);
	return (NODE_OR);
}

void	set_command_data(t_ast_node *node)
{
	node->data.command.exec_data.in_type = STD_T;
	node->data.command.exec_data.out_type = STD_T;
	node->data.command.exec_data.in_file = STDIN_FILENO;
	node->data.command.exec_data.out_file = STDOUT_FILENO;
}
