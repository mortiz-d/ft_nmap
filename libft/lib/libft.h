/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 12:51:39 by mortiz-d          #+#    #+#             */
/*   Updated: 2026/01/11 23:58:58 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stdarg.h>

#define HUFFMAN_MAX_CODE_LENGHT 8

typedef struct s_huffman_count_list
{
	char c;
	int count;
	char code[HUFFMAN_MAX_CODE_LENGHT];
}	t_huffman_count_list;


typedef struct s_binary_tree
{
	int						count;
	int						content;
	struct s_binary_tree	*left; 
	struct s_binary_tree	*right;
} t_binary_tree;


typedef struct s_list
{
	void			*content;
	struct s_list	*next;
	struct s_list	*prev;
}	t_list;

enum flag_type {
    FLAG_INTEGER,	//Needs an argument type integer
    FLAG_STRING,     //Needs an argument type string
	FLAG_CHAR,		//Needs an argument type char
	FLAG_BOOLEAN,	//Needs an argument type boolean
	FLAG_NONE		//Doesnt needs any argument, just the flag presence
};

typedef union s_argument {
    int    i;
    float  f;
    double d;
    char   c;
    char  *s;
} t_argument;

typedef struct s_flag
{
	enum flag_type	type;
	char			*name;
	int 			min_range;
	int 			max_range;
	t_argument		*argument;
	int				status;
	struct s_flag	*next;
	struct s_flag	*prev;
}	t_flag;

int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isalnum(int c);
int			ft_isascii(int c);
int			ft_isprint(int c);
int			ft_strlen(const char *str);
int 		ft_is_exact_word(char *str, char *cmp);
void		*ft_memset(void *str, int c, size_t size);
void		ft_bzero(void *s, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dst, const void *src, size_t len);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t		ft_strlcat(char *dst, const char *src, size_t dstsize);
int			ft_toupper(int c);
int			ft_tolower(int c);
int			ft_count_list(int *list);
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
char		*ft_strnstr(const char *haystack, const char *needle, size_t len);
int			ft_atoi(const char *str);
void		*ft_calloc(size_t count, size_t size);
char		*ft_strdup(const char *s1);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);
char		**ft_split(char const *s, char c);
char		*ft_itoa(int n);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
t_list		*ft_lstnew(void * content);
void		ft_lstadd_front(t_list **lst, t_list *new);
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst);
void		ft_lstclear(t_list **lst);
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstmap(t_list *lst, t_list *lst_start, void *(*f)(t_list*, void *));

//PRINTF
void	ft_putnbr(int nb);
int		ft_printf(const char *c, ...);
int		ft_write_char(int c);
int		ft_write_string(const char *c);
int		ft_write_pointer(const void *c);
int		ft_write_decimal(int number);
int		ft_write_integer(int number);
int		ft_write_unsigned_integer(unsigned int number);
int		ft_write_hexadecimal(unsigned long int num, char * str);
int		ft_write_hexadecimal_XX(unsigned long int num, char * str,int end_loop,int limit_show,char c);
int	    ft_write_size_t(size_t number);

//FLAGS
t_flag *set_up_flag(t_flag *flag,char * name, enum flag_type type,int min_range,int max_range);
t_flag *find_flag(t_flag *flag, char *c);
int check_flag_status(t_flag *start_flag, char *c);
t_argument *get_flag_value(t_flag *start_flag, char *c);
int process_flags(int argc, char **argv, t_flag *flag);
void visualize_flags(t_flag *flag);
void	ft_flagclear(t_flag **lst);

//HUFFMAN
t_list *ft_huffman_find(t_list *lst, char c);
t_list 	*ft_huffman_add_character(t_list *lst, char c);
size_t	ft_huffman_calculate_bitsize(t_list *lst);
size_t 	ft_huffman_list_count(t_list *lst);
void 	ft_huffman_visualize(t_list *lst);
void	fd_huffman_destroy_list(t_list *lst);

//BINARY TREE
t_binary_tree   *ft_binarytree_create(int content);
t_binary_tree   *ft_binarytree_insert(t_binary_tree	*tree, int new_value);
t_binary_tree	*ft_binarytree_minValue(t_binary_tree *tree);
t_binary_tree	*ft_binarytree_maxValue(t_binary_tree *tree);
t_binary_tree	*ft_binarytree_delete(t_binary_tree* tree, int content);
void			ft_binarytree_visualize(t_binary_tree *tree);
// ft_binarytree_visualize



#endif
