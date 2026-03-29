/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_decimal.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 10:35:42 by mortiz-d          #+#    #+#             */
/*   Updated: 2021/11/30 17:34:57 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft.h"

int	ft_write_integer(int number)
{
	int	cont;

	if (number < 0)
		cont = 2;
	else
		cont = 1;
	ft_putnbr(number);
	while (number / 10 != 0)
	{
		number /= 10;
		cont++;
	}
	return (cont);
}

int	ft_write_decimal(int number)
{
	int	cont;

	if (number < 0)
		cont = 2;
	else
		cont = 1;
	ft_putnbr(number);
	while (number / 10 != 0)
	{
		number /= 10;
		cont++;
	}
	return (cont);
}
