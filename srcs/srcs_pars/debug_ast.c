#include "minishell.h"

// Helper function to convert node type to string
static const char *node_type_to_string(t_node_type type)
{
    switch (type)
    {
        case NODE_COMMAND:   return "NODE_COMMAND";
        case NODE_PIPE:      return "NODE_PIPE";
        case NODE_AND:       return "NODE_AND";
        case NODE_SUBSHELL:  return "NODE_SUBSHELL";
        case NODE_OR:        return "NODE_OR";
        default:            return "UNKNOWN_NODE_TYPE";
    }
}

// Helper function to convert redirection type to string
static const char *redir_type_to_string(int type)
{
    switch (type)
    {
        case REDIR_INPUT:    return "REDIR_INPUT";
        case REDIR_OUTPUT:   return "REDIR_OUTPUT";
        case REDIR_APPEND:   return "REDIR_APPEND";
        case REDIR_HEREDOC:  return "REDIR_HEREDOC";
        default:            return "UNKNOWN_REDIR_TYPE";
    }
}

// Helper function to print indentation
static void print_indent(int depth)
{
    for (int i = 0; i < depth; i++)
        printf("  ");
}

// Helper function to print redirections
static void print_redirections(t_redir *redir, int depth)
{
    while (redir)
    {
        print_indent(depth);
        printf("└─ Redirection: %s ", redir_type_to_string(redir->type));
        switch (redir->type)
        {
            case REDIR_INPUT:
                printf("< %s\n", redir->file);
                break;
            case REDIR_OUTPUT:
                printf("> %s\n", redir->file);
                break;
            case REDIR_APPEND:
                printf(">> %s\n", redir->file);
                break;
            case REDIR_HEREDOC:
                printf("<< %s\n", redir->file);
                break;
        }
        redir = redir->next;
    }
}

// Main debug function to print the AST
void debug_print_ast(t_ast_node *node, int depth)
{
    printf("\nAST Structure:\n");
    if (!node)
    {
        print_indent(depth);
        printf("(NULL node)\n");
        return;
    }

    print_indent(depth);
    printf("Node type: %s\n", node_type_to_string(node->type));

    switch (node->type)
    {
        case NODE_COMMAND:
            print_indent(depth);
            printf("Command: '%s'\n", node->u_data.s_command.command ?
                   node->u_data.s_command.command : "(null)");
            
            // Print args array
            if (node->u_data.s_command.args)
            {
                print_indent(depth + 1);
                printf("Args (%d): ", node->u_data.s_command.arg_count);
                for (int i = 0; i < node->u_data.s_command.arg_count; i++)
                    printf("'%s' ", node->u_data.s_command.args[i]);
                printf("\n");
            }
            
            // Print argv_exec array
            if (node->u_data.s_command.argv_exec)
            {
                print_indent(depth + 1);
                printf("Argv_exec: ");
                for (int i = 0; node->u_data.s_command.argv_exec[i]; i++)
                    printf("'%s' ", node->u_data.s_command.argv_exec[i]);
                printf("\n");
            }
            
            // Print redirections
            if (node->redirections)
            {
                print_indent(depth + 1);
                printf("Redirections:\n");
                print_redirections(node->redirections, depth + 2);
            }
            break;

        case NODE_PIPE:
            printf("Pipe\n");
            if (node->redirections)
                print_redirections(node->redirections, depth + 1);
            print_indent(depth);  // These indentations should be adjusted
            printf("└─ Left:\n");
            debug_print_ast(node->u_data.s_pipe.left, depth + 2);
            print_indent(depth);  // to maintain consistent visual hierarchy
            printf("└─ Right:\n");
            debug_print_ast(node->u_data.s_pipe.right, depth + 2);
            break;

        case NODE_SUBSHELL:
            printf("Subshell\n");
            if (node->redirections)
                print_redirections(node->redirections, depth + 1);
            debug_print_ast(node->u_data.s_subshell.command, depth + 1);
            break;

        case NODE_AND:
            printf("AND\n");
            if (node->redirections)
                print_redirections(node->redirections, depth + 1);
            print_indent(depth);
            printf("└─ Left:\n");
            debug_print_ast(node->u_data.s_logical_op.left, depth + 2);
            print_indent(depth);
            printf("└─ Right:\n");
            debug_print_ast(node->u_data.s_logical_op.right, depth + 2);
            break;

        case NODE_OR:
            printf("OR\n");
            if (node->redirections)
                print_redirections(node->redirections, depth + 1);
            print_indent(depth);
            printf("└─ Left:\n");
            debug_print_ast(node->u_data.s_logical_op.left, depth + 2);
            print_indent(depth);
            printf("└─ Right:\n");
            debug_print_ast(node->u_data.s_logical_op.right, depth + 2);
            break;
    }
}
