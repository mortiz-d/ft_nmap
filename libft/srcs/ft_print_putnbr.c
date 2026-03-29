/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 10:15:49 by mortiz-d          #+#    #+#             */
/*   Updated: 2021/11/22 16:31:02 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft.h"

void	ft_putnbr(int nb);
void	ft_putchar(char c);
void	alznum(int n);

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	alznum(int n)
{	
	int	x;

	x = n % 10;
	if (n / 10 != 0)
		alznum(n / 10);
	ft_putchar(x + '0');
}

void	ft_putnbr(int nb)
{
	if (nb <= 2147483647 && nb >= 0)
		alznum(nb);
	else if (nb < 0)
	{
		if (nb == -2147483648)
		{
			nb = nb / 10;
			nb = nb * -1;
			ft_putchar('-');
			alznum(nb);
			ft_putchar('8');
		}
		else
		{
			nb = nb * -1;
			ft_putchar('-');
			alznum(nb);
		}		
	}
}