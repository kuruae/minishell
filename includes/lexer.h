/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:10:53 by enzo              #+#    #+#             */
/*   Updated: 2024/11/15 16:53:01 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

/* Token types for lexer */
typedef enum e_token_type
{
    TOK_WORD,       // Regular word or command
    TOK_OPERATOR,   // |, ||, &, &&, etc.
    TOK_REDIR,      // <, >, >>, <<
    TOK_PIPE,       // |
    TOK_AND,        // &&
    TOK_OR,         // ||
    TOK_NEWLINE,    // \n
    TOK_EOF         // End of input
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
    t_token_type    type;
    char            *value;
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

#endif