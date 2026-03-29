/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_string.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 10:44:15 by mortiz-d          #+#    #+#             */
/*   Updated: 2021/11/23 17:39:57 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft.h"

int	ft_write_char(int c)
{
	write (1, &c, 1);
	return (1);
}


int	ft_write_string(const char *c)
{
	int	cont;

	cont = 0;
	if (c == NULL)
	{
		write(1, "(nill)", 6);
		return (6);
	}
	while (c[cont] != 0)
	{
		ft_write_char(c[cont]);
		cont++;
	}
	return (cont);
}
