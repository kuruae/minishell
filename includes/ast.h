#ifndef AST_H
# define AST_H

/* Node types for the AST */
typedef enum e_node_type
{
    NODE_COMMAND,    // Simple command
    NODE_PIPE,       // Pipe operator (|)
    NODE_REDIR,      // Redirections (<, >, >>)
    NODE_AND,        // Logical AND (&&)
    NODE_OR,         // Logical OR (||)
    NODE_SEQUENCE    // Command sequence (;)
} t_node_type;

/* Redirection types */
typedef enum e_redir_type
{
    REDIR_INPUT,     // <
    REDIR_OUTPUT,    // >
    REDIR_APPEND,    // >>
    REDIR_HEREDOC    // <<
} t_redir_type;

/* Structure for command arguments */
typedef struct s_command
{
    char            **args;      // Array of command arguments (args[0] is the command)
    t_word_props    *word_props; // Properties for each argument
    int             argc;        // Number of arguments
} t_command;

/* Structure for redirections */
typedef struct s_redirection
{
    t_redir_type    type;       // Type of redirection
    char            *file;       // File name or delimiter for heredoc
    t_word_props    file_props; // Properties of the file name
} t_redirection;

/* Main AST node structure */
typedef struct s_ast_node
{
    t_node_type         type;       // Type of the node
    struct s_ast_node   *left;      // Left child
    struct s_ast_node   *right;     // Right child
    t_command           *cmd;       // Command data (if type == NODE_COMMAND)
    t_redirection       *redir;     // Redirection data (if type == NODE_REDIR)
} t_ast_node;

#endif