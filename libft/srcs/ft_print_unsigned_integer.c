/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_unsigned_integer.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 11:29:42 by mortiz-d          #+#    #+#             */
/*   Updated: 2021/11/30 17:35:28 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft.h"

int	ft_write_unsigned_integer(unsigned int number)
{	
	int	cont;

	cont = 1;
	if (number > 2147483647)
	{
		ft_putnbr((int)(number / 10));
		ft_putnbr((int)(number % 10));
	}
	else
		ft_putnbr((int)number);
	while (number / 10 != 0)
	{
		number /= 10;
		cont++;
	}
	return (cont);
}
