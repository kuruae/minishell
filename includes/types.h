/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 21:18:16 by enzo              #+#    #+#             */
/*   Updated: 2025/02/07 21:34:01 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include <stdbool.h>
# include <sys/types.h>

/* Forward declarations */
typedef struct s_ast_node	t_ast_node;
typedef struct s_shell		t_shell;
typedef struct s_token		t_token;
typedef struct s_parser		t_parser;
typedef struct s_exec_data	t_exec_data;
typedef struct s_directory	t_directory;

/* Shared enums */
typedef enum e_error
{
	SUCCESS,
	CTRL_D,
	ERR_FATAL,
	ERR_MALLOC,
	ERR_SYNTAX,
	FAILURE,
	CTRL_C = 130
}	t_error;

typedef enum e_exec_error
{
	EXEC_ERR_FATAL,
	EXEC_ERR_NON_FATAL,
	EXEC_ERR_ACCESS,
	EXEC_ERR_FILE,
	EXEC_SUCCESS,
	EXEC_NOT_FOUND,
	EXEC_ERR_PIPE
}	t_exec_error;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_SUBSHELL,
	NODE_OR,
}	t_node_type;

typedef enum e_token_type
{
	TOK_WORD,
	TOK_OPERATOR,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_HEREDOC,
	TOK_APPEND,
	TOK_PIPE,
	TOK_AND,
	TOK_OR,
	TOK_PAR_OPEN,
	TOK_PAR_CLOSE,
	TOK_NEWLINE,
	TOK_EOF,
}	t_token_type;

#endif