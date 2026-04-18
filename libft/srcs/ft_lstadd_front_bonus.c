/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguelangelortizdelburgo <miguelangelor    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 13:20:10 by mortiz-d          #+#    #+#             */
/*   Updated: 2024/12/05 19:23:20 by miguelangel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft.h"
void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return;

	if (*lst)
	{
		new->next = *lst;
		(*lst)->prev = new;
	}
	else
		new->next = NULL;

	new->prev = NULL;
	*lst = new;
}
