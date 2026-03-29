/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguelangelortizdelburgo <miguelangelor    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 14:45:43 by mortiz-d          #+#    #+#             */
/*   Updated: 2024/12/05 19:23:20 by miguelangel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*aux1;
	int		auxsize;

	if (s1 == NULL || s2 == NULL)
		return (0);
	auxsize = ft_strlen((char *)s1) + ft_strlen((char *)s2) + 1;
	aux1 = ft_calloc(sizeof(char), auxsize);
	if (aux1 == NULL)
		return (0);
	ft_strlcpy(aux1, (char *)s1, auxsize);
	ft_strlcat(aux1, (char *)s2, auxsize);
	return (aux1);
}
