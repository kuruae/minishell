######### COLORS ########

GREEN := $(shell printf "\033[32m")
RED := $(shell printf "\033[31m")
BLUE := $(shell printf "\033[34m")
YELLOW := $(shell printf "\033[33m")
RESET := $(shell printf "\033[0m")
BOLD := $(shell printf "\033[1m")

######### UNICODE CHARS ########

CHECK := "✓"
ARROW := "➜"

######### FLAGS ########

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
SANITIZE_FLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer
LIBFT_PATH = libft

ifeq ($(shell uname), Darwin)
	READLINE_FLAGS = -lreadline -L$(shell brew --prefix readline)/lib -I$(shell brew --prefix readline)/include
else
    READLINE_FLAGS = -lreadline -ltermcap
endif

######### DIRECTORIES ########

SRCS_DIR = srcs
SRCS_EXEC_DIR = $(SRCS_DIR)/exec
SRCS_PARS_DIR = $(SRCS_DIR)/parsing
SRCS_BUILTIN_DIR = $(SRCS_DIR)/builtin
SRCS_MAIN_DIR = $(SRCS_DIR)/main
SRCS_SIGNALS_DIR = $(SRCS_DIR)/signals
OBJ_DIR = objs
INCLUDE_DIR = includes
INCLUDE_DIRS = -I$(INCLUDE_DIR) -I$(LIBFT_PATH)/includes

######### FILES ########
## Builtin source files
SRCS_BUILTIN = $(addprefix $(SRCS_BUILTIN_DIR)/, \
    builtin.c \
    ft_cd.c \
    ft_echo.c \
    ft_env.c \
    ft_exit.c \
    ft_export.c \
    ft_pwd.c \
    ft_unset.c)

## Exec source files
SRCS_EXEC = $(addprefix $(SRCS_EXEC_DIR)/, \
    command_tools.c \
    exec_commands.c \
    exec_pipe.c \
    exec_tools.c \
    exec_tools2.c \
    exec_tools3.c \
    exec.c \
    set_in_out_files.c \
    set_pipes.c)

## Main source files
SRCS_MAIN = $(addprefix $(SRCS_MAIN_DIR)/, \
    clean_up.c \
    free_tokens_and_ast.c \
    main.c)

## AST source files
SRCS_AST = $(addprefix $(SRCS_PARS_DIR)/ast/, \
    ast_argv_exec.c \
    ast_command.c \
    ast_expand_handler.c \
    ast_free.c \
    ast_logic.c \
    ast_pipe.c \
    ast_redir.c \
    ast_remove_quotes.c \
    ast_subshell.c \
    ast_utils.c \
    ast.c)

## Expansions source files
SRCS_EXPANSIONS = $(addprefix $(SRCS_PARS_DIR)/expansions/, \
    expansion_quote_utils.c \
    expansion_start.c \
    expansion.c \
    wildcard_starter.c \
    wildcard_utils.c \
    wildcard.c)

## Heredoc source files
SRCS_HEREDOC = $(addprefix $(SRCS_PARS_DIR)/heredoc/, \
    heredoc_rand_name.c \
    heredoc_utils.c \
    heredoc.c)

## Lexing source files
SRCS_LEXING = $(addprefix $(SRCS_PARS_DIR)/lexing/, \
    lexing_grammar_subshells.c \
    lexing_grammar_utils.c \
    lexing_grammar.c \
    lexing_token_utils.c \
    lexing_utils.c \
    lexing.c)

## Signals source files
SRCS_SIGNALS = $(addprefix $(SRCS_SIGNALS_DIR)/, \
    get_signal_handlers.c \
    get_signal.c)

## Combine all source files
SRC_FILES = $(SRCS_BUILTIN) $(SRCS_EXEC) $(SRCS_MAIN) $(SRCS_AST) \
            $(SRCS_EXPANSIONS) $(SRCS_HEREDOC) $(SRCS_LEXING) $(SRCS_SIGNALS)

## object files
OBJ_FILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRC_FILES)))

######### LIBRARIES ########

LIBFT = $(LIBFT_PATH)/libft.a

######### NAME ########

NAME = minishell
NAME_SANITIZE = $(NAME)_sanitize

######### PROGRESS BAR ########

define progress_bar_parsing
    @printf "\r$(BOLD)$(YELLOW)Parsing 🚘 $(GREEN)"; \
    for p in "⛽█░░░░" "⛽██░░░" "⛽███░░" "⛽████░"; do \
        printf "\r$(BOLD)$(YELLOW)Parsing [$(GREEN)%s$(YELLOW)] Filling tank...$(RESET)" "$$p"; \
        sleep 0.05; \
    done
endef

define progress_bar_exec
    @printf "\r$(BOLD)$(RED)Executing 🚙 $(GREEN)"; \
    for p in "⛽█░░░░" "⛽██░░░" "⛽███░░" "⛽████░"; do \
        printf "\r$(BOLD)$(RED)Executing [$(GREEN)%s$(RED)] Filling tank...$(RESET)" "$$p"; \
        sleep 0.05; \
    done
endef

define progress_bar_builtin
    @printf "\r$(BOLD)$(BLUE)Builtin 🚐 $(GREEN)"; \
    for p in "⛽█░░░░" "⛽██░░░" "⛽███░░" "⛽████░"; do \
        printf "\r$(BOLD)$(BLUE)Builtin [$(GREEN)%s$(BLUE)] Filling tank...$(RESET)" "$$p"; \
        sleep 0.05; \
    done
endef

######### COMMANDS ########

all: $(NAME)

$(NAME): $(OBJ_FILES) $(LIBFT)
	@printf "$(YELLOW)$(ARROW) Linking objects...$(RESET)\n"
	@$(CC) $(CFLAGS) -o $@ $^ -L$(LIBFT_PATH) -lft $(READLINE_FLAGS)
	@printf "$(GREEN)$(CHECK) $(NAME) successfully compiled!$(RESET)\n"

# Sanitized build target
sanitize: CFLAGS += $(SANITIZE_FLAGS)
sanitize: $(NAME_SANITIZE)

$(NAME_SANITIZE): $(OBJ_FILES) $(LIBFT)
	@printf "$(YELLOW)$(ARROW) Linking objects with sanitizer...$(RESET)\n"
	@$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -o $@ $^ -L$(LIBFT_PATH) -lft  $(READLINE_FLAGS)
	@printf "$(GREEN)$(CHECK) $(NAME) with sanitizer successfully compiled!$(RESET)\n"
	@printf "$(YELLOW)$(ARROW) Run with: ./$(NAME_SANITIZE)$(RESET)\n"

$(OBJ_DIR)/%.o: $(SRCS_MAIN_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@
	@$(call progress_bar_parsing)

$(OBJ_DIR)/%.o: $(SRCS_SIGNALS_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@
	@$(call progress_bar_parsing)

$(OBJ_DIR)/%.o: $(SRCS_PARS_DIR)/*/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@
	@$(call progress_bar_parsing)

$(OBJ_DIR)/%.o: $(SRCS_EXEC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@
	@$(call progress_bar_exec)

$(OBJ_DIR)/%.o: $(SRCS_BUILTIN_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@
	@$(call progress_bar_builtin)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFT):
	@printf "$(YELLOW)$(ARROW) Building libft...$(RESET)\n"
	@$(MAKE) -C $(LIBFT_PATH) --no-print-directory > /dev/null 2>&1 & \
	pid=$$!; \
	printf "\r$(BLUE)Building libft$(RESET) ["; \
	while kill -0 $$pid 2>/dev/null; do \
		printf "\r$(BLUE)Building libft$(RESET) [$(GREEN)█$(RESET)]"; \
		sleep 0.1; \
		printf "\r$(BLUE)Building libft$(RESET) [$(GREEN)█▒$(RESET)]"; \
		sleep 0.1; \
		printf "\r$(BLUE)Building libft$(RESET) [$(GREEN)█▒▒$(RESET)]"; \
		sleep 0.1; \
	done; \
	wait $$pid; \
	printf "\n$(GREEN)$(CHECK) libft successfully compiled!$(RESET)\n"

clean:
	@printf "$(RED)$(ARROW) Cleaning object files...$(RESET)\n"
	@$(MAKE) -C $(LIBFT_PATH) clean >/dev/null 2>&1
	@rm -rf $(OBJ_DIR)
	@printf "$(RED)$(CHECK) Object files cleaned!$(RESET)\n"

fclean: clean
	@printf "$(RED)$(ARROW) Removing executables...$(RESET)\n"
	@$(MAKE) -C $(LIBFT_PATH) fclean >/dev/null 2>&1
	@rm -f $(NAME) $(NAME_SANITIZE)
	@printf "$(RED)$(CHECK) Executables removed!$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re sanitize