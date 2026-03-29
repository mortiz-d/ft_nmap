/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguelangelortizdelburgo <miguelangelor    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 17:41:23 by mortiz-d          #+#    #+#             */
/*   Updated: 2024/12/05 19:23:20 by miguelangel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	unsigned int	cont;
	unsigned int	modsize;

	cont = 0;
	while (dst[cont] != '\0' && cont <= dstsize)
		cont++;
	modsize = dstsize - cont;
	if (cont < dstsize)
		return (cont + ft_strlcpy ((dst + cont), src, modsize));
	return (ft_strlen((char *)src) + dstsize);
}
