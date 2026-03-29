/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_hexadecimal_max.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 13:10:13 by mortiz-d          #+#    #+#             */
/*   Updated: 2021/11/30 17:35:04 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft.h"


int	get_long_hexa(unsigned long int num)
{
	unsigned int	aux,auxnum;
	int cont;

	cont = 1;
	auxnum = (unsigned int)num;
	aux = auxnum / 16;

	if (aux != 0)
	{
		cont += get_long_hexa(aux);
	}

	return (cont);
}


int	ft_write_hexadecimal_XX(unsigned long int num, char * str,int start_loop,int limit_show, char c)
{
	unsigned int	temp,aux;
	unsigned long int auxnum;
	int cont, long_hex;

	cont = 1;
	auxnum = num;
	temp = auxnum % 16;
	aux = auxnum / 16;

	if(start_loop)
	{
		long_hex = get_long_hexa(num);
		while(long_hex++ < limit_show)
		{
			ft_write_char (c);
			cont++;
		}
		
	}

	if (aux != 0)
	{
		cont += ft_write_hexadecimal_XX(aux, str,0,limit_show,c);
	}
	if (cont <= limit_show)
		ft_write_char (str[temp]);

	return (cont);
}


int	ft_write_hexadecimal(unsigned long int  num, char * str)
{
	unsigned int	temp;
	unsigned int	aux;
	unsigned int	cont;
	unsigned long int	auxnum;

	cont = 1;
	auxnum = (unsigned long int)num;
	temp = auxnum % 16;
	aux = auxnum / 16;
	if (aux != 0)
	{
		cont += ft_write_hexadecimal(aux, str);
	}
	ft_write_char (str[temp]);
	return (cont);
}