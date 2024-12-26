/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:10:53 by enzo              #+#    #+#             */
/*   Updated: 2024/12/27 00:27:42 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXING_H
# define LEXING_H

# include "minishell.h"

/* Token types for lexer */
typedef enum e_token_type
{
    TOK_WORD,       // Regular word or command
    TOK_OPERATOR,   // |, ||, &, &&, etc.
    TOK_REDIR_IN,   // <
    TOK_REDIR_OUT,  // >
    TOK_HEREDOC,    // << (heredoc)
    TOK_APPEND,     // >> (append)
    TOK_PIPE,       // |
    TOK_AND,        // &&
    TOK_OR,         // ||
    TOK_PAR_OPEN,   // (
    TOK_PAR_CLOSE,  // )
    TOK_NEWLINE,    // \n
    TOK_EOF,        // End of input
    TOK_EXPAND,     // $
    TOK_WILDCARD    // *
} t_token_type;

/* Quote state for parsing */
typedef enum e_quote_state
{
    QUOTE_NONE,
    QUOTE_SINGLE,    // '
    QUOTE_DOUBLE     // "
} t_quote_state;

/* Token structure */
typedef struct s_token
{
    char            *value;
    t_token_type    type;
    struct s_token  *next;
} t_token;

/* Word (command/argument) properties */
typedef struct s_word_props
{
    bool    has_wildcard;     // Contains * or ?
    bool    in_single_quotes; // Was in single quotes
    bool    in_double_quotes; // Was in double quotes
    char    *original;        // Original word before expansion
    char    *expanded;        // Word after expansion
} t_word_props;

/* Lexer functions */

t_token *lexing(char *line);
t_token *get_all_tokens_from_word(char *line);
t_token *create_token(char *value, size_t len, t_token_type type);
size_t	get_token_len(char *line, t_token_type type);
void	free_tokens(t_token *tokens);
void	add_new_token(t_token **tokens, t_token *new_token);
int     get_tok_word_len(char *line);
t_error verify_unclosed_quotes(t_token *tokens);


#endif