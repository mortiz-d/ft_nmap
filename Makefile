# COLORS #
GREEN = @echo "\033[0;32m"
BLUE = @echo "\033[0;34m" 
PURPLE = @echo "\033[0;35m"
CYAN = @echo "\033[0;36m"
RESET = "\033[1;0m"

# RESOURCES #
SRCS =	srcs/main.c \
		srcs/debug.c \
		srcs/params.c \
		srcs/flags.c 

# FLAGS #
OBJS_DIR = objetos
OBJS = $(SRCS:srcs/%.c=$(OBJS_DIR)/%.o)

NAME = ft_nmap
COMMAND = 

CC = gcc

CFLAGS = -std=c99   -Wall -Werror -Wextra
LIBFT_A = libft/libft.a

# Instructions #
all: $(NAME)

$(NAME): $(OBJS)
	@make -C ./libft
#
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT_A)
	$(BLUE) $(NAME) set Up $(RESET)
#	mv ./$(NAME) ./testing/files/$(NAME)

$(OBJS_DIR)/%.o: srcs/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -g -c $<  -o $@
	$(CYAN) $(NAME) Object Compiled $< $(RESET)

clean:
	@make fclean -s -C./libft
	@rm -rf $(OBJS_DIR)
	$(PURPLE) Cleaned $(NAME) Objects $(RESET)

fclean: clean
#	@rm ./testing/files/$(NAME)
	@rm $(NAME)
	$(PURPLE) Cleaned $(NAME) Executable $(RESET)	

re: fclean all

test: re
	clear
	sudo ./testing/files/test_command.sh

.PHONY: all clean fclean re test



