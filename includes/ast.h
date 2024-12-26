#ifndef AST_H
# define AST_H

# include "minishell.h"

/* Node types for the AST */
typedef enum e_node_type
{
	NODE_COMMAND,    // Simple command
	NODE_PIPE,       // Pipe operator (|)
	// NODE_REDIR,      // Redirections (<, >, >>)
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
} t_redir;

typedef struct s_parser
{
	t_token     *current;   // Current token being processed
	t_token     *tokens;    // All tokens
	t_error     err_status; // Track parsing errors
} t_parser;

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
			struct s_ast_node	*command;
		} subshell;

		struct
		{
			struct s_ast_node	*left;
			struct s_ast_node	*right;
		} logical_op;
	} data;
} t_ast_node;


bool	_parser_is_token_type_redir(t_token_type type);
t_redir	*create_redir_node(t_token *token, char *file);
t_ast_node *create_ast_node(t_node_type type);
void free_ast(t_ast_node *node);
void	paser_advance(t_parser *parser);
void	add_redir(t_ast_node *node, t_redir *redir);
bool	parse_redir(t_parser *parser, t_ast_node *node);
t_ast_node	*parse_command(t_parser *parser);
t_ast_node	*parse_subshell(t_parser *parser);
int	count_args(t_token *token);
t_ast_node	*parse_pipe(t_parser *parser);
t_ast_node	*parse_logic(t_parser *parser);
void free_ast(t_ast_node *node);
t_ast_node	*parse_tokens(t_token *tokens);
void debug_print_ast(t_ast_node *node, int depth);
t_ast_node	*err_free_and_return(t_parser *parser, t_ast_node *node);
t_node_type	get_node_type(t_token_type token_type);

#endif