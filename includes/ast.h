#ifndef AST_H
# define AST_H

# define HEX_STRING_LEN 8
# define HEREDOC_PREFIX "/tmp/heredoc_"
# define HEX_MASK 0xF

# include "minishell.h"

# include "expansion.h"

/* Node types for the AST */
typedef enum e_node_type
{
	NODE_COMMAND,    // Simple command
	NODE_PIPE,       // Pipe operator (|)
	NODE_AND,        // Logical AND (&&)
	NODE_SUBSHELL,   // Subshell
	NODE_OR,         // Logical OR (||)
} t_node_type;

/* Redirection types */
typedef struct s_redir
{
	enum
	{
		REDIR_INPUT,     // <
		REDIR_OUTPUT,    // >
		REDIR_APPEND,    // >>
		REDIR_HEREDOC    // <<
	} type;
	char			*file;  // File to redirect to/from
	struct s_redir	*next;  // Next redirection
	struct s_redir	*head;  // Head of the redirection list
} t_redir;

typedef struct s_parser
{
	t_token     *current;   // Current token being processed
	t_token     *tokens;    // All tokens
	t_error     err_status; // Track parsing errors
	char		**env;
} t_parser;

typedef struct s_sig
{
	volatile sig_atomic_t	sig;
}	t_sig;

/* Structure for command arguments */
typedef struct s_ast_node 
{
	t_node_type	type;  // Type of the node    
	t_redir		*redirections;  // All nodes can have redirections
	union
	{
		struct
		{
			char		*command;
			char		**args;
			char		**argv_exec;
			int			arg_count;
			t_exec_data	exec_data;
		} command;

		struct
		{
			struct s_ast_node	*left;
			struct s_ast_node	*right;
		} pipe;

		struct
		{
			struct s_ast_node	*command; // a bit confusing, because this can be pipe or logical operator
		} subshell;

		struct
		{
			struct s_ast_node	*left;
			struct s_ast_node	*right;
		} logical_op;
	} data;
} t_ast_node;

/* AST Node Management */
t_ast_node		*create_ast_node(t_node_type type);
t_node_type		get_node_type(t_token_type token_type);
void			free_ast(t_ast_node *node);
void			set_command_data(t_ast_node *node);

/* Parsing Functions */
int				count_args(t_token *token);
void			paser_advance(t_parser *parser);
bool			parse_redir(t_parser *parser, t_ast_node *node);
bool			_parser_is_token_type_redir(t_token_type type);
t_ast_node		*ast_handler(t_token *tokens, char ***env);
t_ast_node		*err_free_and_return(t_parser *parser, t_ast_node *node);
t_ast_node		*parse_command(t_parser *parser);
t_ast_node		*parse_logic(t_parser *parser);
t_ast_node		*parse_pipe(t_parser *parser);
t_ast_node		*parse_subshell(t_parser *parser);
t_error			pointer_parse_pipe(t_parser *parser, t_ast_node **left);

/* Quote and Expansion Handlers */
t_error			all_expands_handler(t_ast_node *node, char **env);
t_error			create_argv_exec(t_ast_node *node);
t_error			remove_quotes_handler(t_ast_node *node);
bool			is_expansion_on(char *delemiter);
char			*remove_quotes_from_string(char *str);

/* Heredoc Functions */
bool			check_delim_match(char *input, char *delimiter);
bool			is_expansion_on(char *delemiter);
char			*get_heredoc_filename(void);
char			*heredoc_handler(char *delimiter, char **env);
void			uint_to_hex(unsigned int num, char *hex_str);
unsigned int	lcg_rand(unsigned int *seed);

/* Debug */
void			debug_print_ast(t_ast_node *node, int depth);

#endif