/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguelangelortizdelburgo <miguelangelor    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 19:36:19 by mortiz-d          #+#    #+#             */
/*   Updated: 2024/12/05 19:23:20 by miguelangel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft.h"
void	ft_lstclear(t_list **lst)
{
	t_list	*aux;
	t_list	*aux2;

	aux = *lst;
	while (aux != 0)
	{
		aux2 = aux->next;
		aux->content = 0;
		free(aux);
		aux = aux2;
	}
	*lst = 0;
}
