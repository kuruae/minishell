/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 21:18:16 by enzo              #+#    #+#             */
/*   Updated: 2025/02/08 16:56:40 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include <stdbool.h>
# include <sys/types.h>

/* Shared error codes */
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

/* Token types for the lexer */
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

/* Quote state for parsing */
typedef enum e_quote_state
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	t_quote_state;

/* Node types for the AST */
typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_SUBSHELL,
	NODE_OR,
}	t_node_type;

/* Execution error codes */
typedef enum e_exec_error
{
	EXEC_ERR_FATAL,
	EXEC_ERR_NON_FATAL,
	EXEC_ERR_ACCESS,
	EXEC_ERR_FILE,
	EXEC_SUCCESS,
	EXEC_NOT_FOUND,
	EXEC_ERR_PIPE,
	EXEC_NEUTRAL
}	t_exec_error;

#endif