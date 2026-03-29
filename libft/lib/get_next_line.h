/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 13:47:28 by mortiz-d          #+#    #+#             */
/*   Updated: 2026/01/13 00:08:32 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

char		*get_next_line(int fd);
int			ft_write_char(int c);
int			ft_write_string(const char *c);
int			ft_strlen_gnl(const char *str);
char		*ft_strchr_gnl(const char *s, int c);
void		*ft_memcpy_gnl(void *dest, const void *src, size_t n);
void		*ft_calloc_gnl(size_t count, size_t size);
void		ft_bzero_gnl(void *s, size_t n);
#endif