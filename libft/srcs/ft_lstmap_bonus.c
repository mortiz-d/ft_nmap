/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguelangelortizdelburgo <miguelangelor    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 21:48:02 by mortiz-d          #+#    #+#             */
/*   Updated: 2024/12/05 19:23:20 by miguelangel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft.h"

static	void	ft_free_all(t_list *start);

t_list	*ft_lstmap(t_list *lst, t_list *lst_start, void *(*f)(t_list*, void *))
{
	t_list	*aux;
	t_list	*start;

	if (lst == NULL)
		return (0);
	aux = ft_lstnew(f(lst_start, lst->content));
	if (aux == NULL)
		return (0);
	start = aux;
	while (lst != NULL)
	{
		lst = lst->next;
		if (lst != NULL)
		{
			aux->next = ft_lstnew(f(lst_start, lst->content));
			aux->next->prev = aux;
			aux = aux->next;
		}
		if (aux == NULL)
		{
			ft_lstclear(&start);
			ft_free_all(start);
			return (NULL);
		}
	}
	return (start);
}

static	void	ft_free_all(t_list *start)
{
	t_list	*aux;

	while (start != NULL)
	{
		aux = start;
		start = start->next;
		free(aux);
	}
	free(start);
}
