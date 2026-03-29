/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_pointer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 10:45:02 by mortiz-d          #+#    #+#             */
/*   Updated: 2021/11/30 17:35:20 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft.h"

// static int	ft_write_hexadecimal_p(unsigned long int num);
// static int	get_long_hexa(unsigned long int num);

int	ft_write_pointer(const void *c)
{
	unsigned long int	pnt;
	int					cont;//, long_hex;

	if (c == NULL)
	{
		write(1, "(nill)", 6);
		return (6);
	}
	cont = 2;
	pnt = (unsigned long int)c;
	write (1, "0x", 2);
	cont += ft_write_hexadecimal(pnt,"0123456789abcdef");
	return (cont);
}

// int	get_long_hexa(unsigned long int num)
// {
// 	unsigned int	aux,auxnum;
// 	int cont;

// 	cont = 1;
// 	auxnum = (unsigned int)num;
// 	aux = auxnum / 16;

// 	if (aux != 0)
// 	{
// 		cont += get_long_hexa(aux);
// 	}
// 	return (cont);
// }

// static int	ft_write_hexadecimal(unsigned long int num)
// static int	ft_write_hexadecimal_p(unsigned long int num)
// {
// 	unsigned long int	temp;
// 	unsigned long int	aux;
// 	int					cont;

// 	cont = 1;
// 	temp = num % 16;
// 	aux = num / 16;
// 	if (aux != 0)
// 	{
// 		cont += ft_write_hexadecimal_p(aux);
// 	}
// 	if (temp < 10)
// 		ft_write_char ((char)((int)temp + 48));
// 	else
// 		ft_write_char ((char)((int)temp + 87));
// 	return (cont);
// }
