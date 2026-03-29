/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguelangelortizdelburgo <miguelangelor    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 17:59:08 by mortiz-d          #+#    #+#             */
/*   Updated: 2024/12/05 19:23:20 by miguelangel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	int		cont;
	int		size;

	if (s != 0 && f != 0)
	{
		cont = 0;
		size = ft_strlen((char *)s);
		while (cont < size)
		{
			f(cont, &s[cont]);
			cont++;
		}
	}
}
