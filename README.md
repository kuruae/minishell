# Minishell

An (almost) POSIX-compliant shell implementation supporting advanced features including subshells, logical operators, and a recursive AST-based parser.

## Features

### Core Functionality
- **Command execution** with absolute/relative paths and `PATH` lookup
- **Builtins**: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- **Redirections**:
  - `>` (overwrite)
  - `>>` (append)
  - `<` (input)
  - `<<` (here document)
- **Pipes** with `|` for multi-stage pipelines
- **Subshells** using `()` syntax
- **Logical operators**: `&&` (AND), `||` (OR)
- **Signal handling**:
  - `Ctrl-C` interrupts foreground process
  - `Ctrl-\` quits frozen processes
  - `Ctrl-D` handles EOF gracefully
- **Environment variables** expansion with `$VAR` syntax
- **Wildcards** (*) for filename expansion (bonus)

## Implementation Details

### Recursive AST Architecture
The parser implements a **recursive descent algorithm** that constructs an Abstract Syntax Tree (AST) through hierarchical grammar analysis

Key characteristics:

- **Context-aware parsing** with operator precedence:

        1. Parentheses `()` for subshells

        2. Logical operators `&&`/`||`

        3. Pipes `|`

- **Left-recursive structure** for logical operators

- **Node types:**

        ``NODE_COMMAND``: Executable with arguments

        ``NODE_PIPE``: Pipeline connector

        ``NODE_AND``: AND operator (&&)

        ``NODE_OR``: OR operator (||) 

        ``NODE_SUBSHELL``: Nested execution context

 - **Redirections** are then applied individually to each command in a pipeline and follow a left-to-right execution order. To do so, each command group has a linked list of redirections, correctly handling various chains of them.

Example AST structure for ``(ls | wc) && make re:``

```
          NODE_AND
         /        \
   NODE_SUBSHELL   NODE_COMMAND
     /      \         |
 NODE_PIPE  ...     "make re"
  /     \
ls     wc
```

**Memory Management**

- **Recursively memory cleanup** of the AST after each processed inputs or failure

- **No garbage collector** implemented to better reflect 42's intent of teaching manual memory handling

- **Clean child processes** preventing ``still reachable`` pointers

- Strict **zero-leak policy** extensively tested through Valgrind

**Execution Flow**

1. **Lexical analysis** with token lookahead/lookbehind

2. **Syntax tree construction** via recursive parsing

3. **AST optimization** (merged redirections, simplified logical branches, expansions, ...)

4. **Depth-first execution**:

- Logical operators: Short-circuit evaluation

- Pipes: Forked processes with dup2 syscalls

- Subshells: Recursive execution in child process

## Installation

```bash
git clone https://github.com/kuruae/minishell.git && cd minishell
make
```
**Dependencies**: ``readline`` library (install via brew/apt)

- This choice is a school requirement, and I belive this is the sole reason this project couldn't be considered POSIX-compliant despite my best efforts for portability.

## Usage
```bash
./minishell

# Example commands
echo "Hello $USER" | cat -e
invalid-command || echo "Fallback succeeded"
cat << EOF > tmp_file.txt > file.txt
```

## Special Thanks

Project partner [Jakob Scuzzi](https://github.com/jacobscuzzi) who was responsible for the entire execution part.


