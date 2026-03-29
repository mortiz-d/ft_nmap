/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguelangelortizdelburgo <miguelangelor    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 12:14:08 by mortiz-d          #+#    #+#             */
/*   Updated: 2024/12/05 19:23:20 by miguelangel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft.h"

void	*ft_memset(void *str, int c, size_t size)
{
	char			*aux1;
	unsigned long	aux2;

	aux1 = str;
	aux2 = 0;
	while ((aux2 < size))
	{
		*aux1 = (unsigned char)c;
		aux2++;
		aux1++;
	}
	return (str);
}
