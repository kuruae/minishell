/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:10:53 by enzo              #+#    #+#             */
/*   Updated: 2025/02/07 20:42:08 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXING_H
# define LEXING_H

# include "minishell.h"

/* Token types for lexer */
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

/* Token structure */
typedef struct s_token
{
	bool			expands;
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_par_counts
{
	int		open;
	int		close;
}	t_par_counts;

/* Lexer core */
t_token		*lexing(char *line);
t_token		*get_all_tokens_from_word(char *line);
t_error		grammar_handler(t_token *tokens);

/* Token utils */
t_token		*create_token(char *value, size_t len, t_token_type type);
size_t		get_token_len(char *line, t_token_type type);
int			get_tok_word_len(char *line);
void		free_tokens(t_token *tokens);
void		add_new_token(t_token **tokens, t_token *new_token);

/*grammar utils functions*/
t_error		is_subshell_valid(t_token *tokens);
t_error		prev_and_after_par(t_token *current, t_token *prev);
bool		is_operator(t_token_type type);
bool		is_redir(t_token_type type);
bool		is_unclosed_quote(char *str);

#endif