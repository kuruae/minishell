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

SRCS_EXEC_DIR = srcs/srcs_exec
SRCS_PARS_DIR = srcs/srcs_pars
SRCS_BUILTIN_DIR = srcs/srcs_builtin
OBJ_DIR = objs
INCLUDE_DIR = includes
INCLUDE_DIRS = -I$(INCLUDE_DIR) -I$(LIBFT_PATH)/includes

######### FILES ########
## source files from all directories
SRC_FILES = $(wildcard $(SRCS_EXEC_DIR)/*.c) \
            $(wildcard $(SRCS_PARS_DIR)/*.c) \
            $(wildcard $(SRCS_BUILTIN_DIR)/*.c)

## object files
OBJ_FILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRC_FILES)))

##
TOTAL_FILES := $(words $(SRC_FILES))
CURRENT_FILE = 1

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

$(OBJ_DIR)/%.o: $(SRCS_PARS_DIR)/%.c | $(OBJ_DIR)
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
