NAME = minishell

SRC_PATH = src/
TOKEN_PATH = token/
PARSING_PATH = parsing/
EXPAND_PATH = expand/
UTILS_PATH = utils/

OBJ_PATH = build/
LIBFT_PATH = libft/
LIBFT_AR = $(LIBFT_PATH)libft.a

SRC = \
	  	main.c
TOKEN = \
		char_check.c \
		token_mgmt.c \
		tokenizer.c
PARSING = \
		parsinette.c 
EXPAND = \
		expand.c 
UTILS = \
		len.c \
		print.c \
		struct.c 

ALL_SRC = $(addprefix $(SRC_PATH), $(SRC)) \
		  $(addprefix $(TOKEN_PATH), $(TOKEN)) \
		  $(addprefix $(PARSING_PATH), $(PARSING)) \
		  $(addprefix $(EXPAND_PATH), $(EXPAND)) \
		  $(addprefix $(UTILS_PATH), $(UTILS))

#SRCS = $(addprefix $(SRC_PATH), $(SRC))
OBJ = $(addprefix $(OBJ_PATH), $(ALL_SRC:.c=.o))
DPD = $(addprefix $(OBJ_PATH), $(ALL_SRC:.c=.d))

CC = cc
CFLAGS = -Wall -Wextra -Werror -MMD -g #-fsanitize=address,leak,undefined
INCS = includes/minishell.h
INC = -Iincludes -I$(LIBFT_PATH)/includes

PURPLE = \033[0;35m
NC = \033[0m

all: $(LIBFT_AR) $(NAME)

$(NAME): $(OBJ) $(INCS)
	@echo "\n$(PURPLE)  Compiling Minishell...$(NC)"
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT_AR) $(INC) -lreadline -o $(NAME)
	@echo "\n$(PURPLE)  Ready ✓$(NC)\n"

-include $(DPD)

$(OBJ_PATH)%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(LIBFT_AR):
	@echo "\n$(PURPLE)  Compiling LIBFT...$(NC)"
	@make -s -C $(LIBFT_PATH)
	@echo "\n$(PURPLE)  Done ✓$(NC)"

clean:
	@echo "\n$(PURPLE)  Cleaning object files... $(NC)"
	@rm -rf $(OBJ_PATH)
	@make -s clean -C $(LIBFT_PATH)
	@echo "\n$(PURPLE)  Done ✓$(NC)"

fclean: clean
	@echo "\n$(PURPLE)  Cleaning Minishell...$(NC)"
	@$(RM) $(NAME)
	@make -s fclean -C $(LIBFT_PATH)
	@echo "\n$(PURPLE)  Done ✓$(NC)"

dev: all clean
 
CHAT = { ignore_readline_leaks Memcheck:Leak ... obj:*/libreadline.so.* } { ignore_bin_functions Memcheck:Leak ... obj:/usr/bin/* } { ncurses_termcap Memcheck:Leak match-leak-kinds:reachable fun:rl_make_bare_keymap fun:rl_generic_bind fun:rl_parse_and_bind obj:/usr/lib/x86_64-linux-gnu/libreadline.so.8.2 fun:rl_initialize fun:readline }

ignore:
	@for i in $(CHAT); do \
   		echo $$i >> ignore.supp; \
	done

val:
	@make -s dev
	@if ! [ -f "ignore.supp" ]; then make ignore; fi
	@valgrind --suppressions=./ignore.supp --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --track-fds=yes -s ./minishell

re: fclean all

.PHONY: all clean fclean re dev val ignore
