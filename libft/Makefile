# COLORS #
GREEN = @echo "\033[0;32m"
BLUE = @echo "\033[0;34m" 
PURPLE = @echo "\033[0;35m"
CYAN = @echo "\033[0;36m"
RESET = "\033[1;0m"

# RESOURCES #
SRCS = 	srcs/ft_memset.c 			\
		srcs/ft_bzero.c 			\
		srcs/ft_memcpy.c 			\
		srcs/ft_memmove.c 			\
		srcs/ft_memchr.c 			\
		srcs/ft_memcmp.c 			\
		srcs/ft_strlen.c 			\
		srcs/ft_isalpha.c 			\
		srcs/ft_isdigit.c 			\
		srcs/ft_isalnum.c 			\
		srcs/ft_isascii.c  			\
		srcs/ft_isprint.c 			\
		srcs/ft_toupper.c 			\
		srcs/ft_tolower.c 			\
		srcs/ft_strchr.c 			\
		srcs/ft_strrchr.c 			\
		srcs/ft_strncmp.c  			\
		srcs/ft_strlcpy.c 			\
		srcs/ft_strlcat.c 			\
		srcs/ft_strnstr.c 			\
		srcs/ft_atoi.c 				\
		srcs/ft_calloc.c 			\
		srcs/ft_strdup.c 			\
		srcs/ft_substr.c 			\
		srcs/ft_strjoin.c 			\
		srcs/ft_strtrim.c 			\
		srcs/ft_split.c 			\
		srcs/ft_itoa.c 				\
		srcs/ft_strmapi.c 			\
		srcs/ft_putchar_fd.c  		\
		srcs/ft_putstr_fd.c 		\
		srcs/ft_putendl_fd.c 		\
		srcs/ft_putnbr_fd.c 		\
		srcs/ft_striteri.c 			\
		srcs/ft_count_list.c 		\
		srcs/ft_is_exact_word.c 	\
		srcs/ft_lstnew_bonus.c 		\
		srcs/ft_lstadd_front_bonus.c\
		srcs/ft_lstsize_bonus.c 	\
		srcs/ft_lstlast_bonus.c 	\
		srcs/ft_lstadd_back_bonus.c \
		srcs/ft_lstdelone_bonus.c 	\
		srcs/ft_lstclear_bonus.c 	\
		srcs/ft_lstiter_bonus.c 	\
		srcs/ft_lstmap_bonus.c 		\
		srcs/ft_print_string.c		\
		srcs/ft_print_decimal.c 			\
		srcs/ft_print_decimal.c 			\
		srcs/ft_print_hexadecimal.c 		\
		srcs/ft_print_pointer.c 			\
		srcs/ft_print_putnbr.c 				\
		srcs/ft_print_size_t.c 				\
		srcs/ft_print_unsigned_integer.c	\
		srcs/ft_printf.c					\
		srcs/flag/ft_flag.c					\
		srcs/huffman_compresion/huffman_destroy_list.c	\
		srcs/huffman_compresion/huffman_count_list.c	\
		srcs/huffman_compresion/huffman_calculate_size.c\
		srcs/huffman_compresion/huffman_list.c	\
		srcs/huffman_compresion/huffman_visualize.c	\
		srcs/binary_tree/find_value_binary_tree.c	\
		srcs/binary_tree/delete_value_binary_tree.c	\
		srcs/binary_tree/create_binary_tree.c		\
		srcs/binary_tree/insert_value_binary_tree.c	\
		srcs/binary_tree/visualice_binary_tree.c	\
		srcs/get_next_line/get_next_line.c			\
		srcs/get_next_line/aux_get_next_line.c



# FLAGS #
OBJS = ${SRCS:srcs/%.c=obj/%.o}

NAME = libft.a

CC = gcc

CFLAGS = -fPIC -std=c99 -Wall -Werror -Wextra -g

AR = ar rcs

# INSTRUCTIONS #
all: ${NAME}

${NAME}: ${OBJS}
	${AR} ${NAME} ${OBJS}
	$(GREEN)LIBFT Library Ensambled $(RESET)	

# Rules for object files
obj/%.o: srcs/%.c
	@mkdir -p obj
	@mkdir -p obj/flag
	@mkdir -p obj/huffman_compresion
	@mkdir -p obj/get_next_line
	@mkdir -p obj/binary_tree
	${CC} ${CFLAGS} -c $< -o $@
	$(CYAN)Compiled LIBFT Object $< $(RESET)

clean:
	@rm -rf obj
	$(PURPLE)Cleaned LIBFT Objects $(RESET)

fclean: clean
	@rm -rf ${NAME}

re: fclean all

.PHONY: all clean fclean re